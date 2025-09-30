#include "fat16.h"
#include "drivers/ata/ata.h"
#include "string.h"

// FAT16 present flag
static bool fat_present = false;

// Use static buffers to avoid stack overflow
static uint8_t sector_buf[512];
static uint8_t fat_buf[512];

// BIOS Parameter Block
static bpb_t bpb;
static uint32_t fat_start;
static uint32_t root_start;
static uint32_t data_start;

// --- Helpers ---

static bool read_bpb(void) {
    // Read sector 0
    if (ata_read_sector(0, sector_buf) != 0) return false;

    uint32_t fat_lba_start = 0; // default to sector 0

    // Check for MBR signature at 0x1FE
    if (sector_buf[510] == 0x55 && sector_buf[511] == 0xAA) {
        // Look at partition table entries
        mbr_partition_t* part = (mbr_partition_t*)(sector_buf + 0x1BE);

        for (int i = 0; i < 4; i++) {
            if (part[i].type == 0x06 || part[i].type == 0x0E) { // FAT16
                fat_lba_start = part[i].lba_first;
                break;
            }
        }
    }

    // If no partition table or no FAT16 found, fallback to superfloppy
    // (BPB is at sector 0)
    // fat_lba_start is already 0 if none found

    // Read BPB sector
    if (ata_read_sector(fat_lba_start, sector_buf) != 0) return false;

    // Check boot sector signature
    if (sector_buf[510] != 0x55 || sector_buf[511] != 0xAA) return false;

    // Copy BPB
    memcpy(&bpb, sector_buf + 11, sizeof(bpb));

    if (bpb.bytes_per_sector != 512 || bpb.sectors_per_fat == 0 || bpb.root_entries == 0)
        return false;

    // Compute offsets relative to fat_lba_start
    fat_start  = fat_lba_start + bpb.reserved_sectors;
    root_start = fat_start + (bpb.num_fats * bpb.sectors_per_fat);
    data_start = root_start + ((bpb.root_entries * 32) / 512);

    return true;
}

static bool filename_match(dir_entry_t *entry, const char *name, const char *ext) {
    return (strncmp(entry->name, name, 8) == 0 &&
            strncmp(entry->ext, ext, 3) == 0);
}

// --- Public API ---


bool fat16_init() {
    curX = 0;
    draw_string(fb, fb_width, 0, curLine, "\nInitializing FAT16...\n", colorWhite);

    fat_present = read_bpb();
    if (!fat_present) {
        draw_string(fb, fb_width, 0, curLine, "No FAT16 drive found, continuing in live mode...\n", colorWhite);
    } else {
        draw_string(fb, fb_width, 0, curLine, "FAT16 ready\n", colorWhite);
    }

    draw_string(fb, fb_width, 0, curLine, "BPB values:\n", colorWhite);
    draw_string(fb, fb_width, 0, curLine, "\nBytes per sector: ", colorWhite); draw_dec(bpb.bytes_per_sector);
    draw_string(fb, fb_width, 0, curLine, "\nSectors per cluster: ", colorWhite); draw_dec(bpb.sectors_per_cluster);
    draw_string(fb, fb_width, 0, curLine, "\nReserved sectors: ", colorWhite); draw_dec(bpb.reserved_sectors);
    draw_string(fb, fb_width, 0, curLine, "\nNum FATs: ", colorWhite); draw_dec(bpb.num_fats);
    draw_string(fb, fb_width, 0, curLine, "\nRoot entries: ", colorWhite); draw_dec(bpb.root_entries);
    draw_string(fb, fb_width, 0, curLine, "\nSectors per FAT: ", colorWhite); draw_dec(bpb.sectors_per_fat);

    draw_string(fb, fb_width, 0, curLine, "\nFAT16 ready\n", colorWhite); newLineFB();
    
    return fat_present;
}

// Reads a small file into out_buf; out_size will contain the file size
bool fat16_read_file(const char *name, const char *ext, uint8_t *out_buf, uint32_t *out_size) {
    if (!fat_present) return false; // live mode: no drive, skip

    dir_entry_t *dir = (dir_entry_t*) sector_buf;

    for (uint32_t s = 0; s < (bpb.root_entries * 32) / 512; s++) {
        ata_read_sector(root_start + s, sector_buf);

        for (int i = 0; i < 512 / sizeof(dir_entry_t); i++) {
            if (dir[i].name[0] == 0x00) return false;  // end of entries
            if ((dir[i].attr & 0x0F) == 0x0F) continue; // long name

            if (filename_match(&dir[i], name, ext)) {
                *out_size = dir[i].file_size;
                uint32_t cluster = dir[i].first_cluster;
                uint32_t remaining = *out_size;
                uint8_t *p = out_buf;

                while (cluster < 0xFFF8 && remaining > 0) {
                    uint32_t sector = data_start + (cluster - 2) * bpb.sectors_per_cluster;

                    for (int j = 0; j < bpb.sectors_per_cluster && remaining > 0; j++) {
                        ata_read_sector(sector + j, p);
                        uint32_t copy = remaining < 512 ? remaining : 512;
                        p += copy;
                        remaining -= copy;
                    }

                    // Read next cluster from FAT
                    uint32_t fat_sector = fat_start + ((cluster * 2) / 512);
                    ata_read_sector(fat_sector, fat_buf);
                    uint32_t off = (cluster * 2) % 512;
                    uint16_t next = (uint16_t)(fat_buf[off] | (fat_buf[off + 1] << 8));
                    cluster = next;
                }

                return true;
            }
        }
    }

    return false;
}
