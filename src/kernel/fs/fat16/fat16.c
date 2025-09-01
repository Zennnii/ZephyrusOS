#include "fat16.h"
#include "drivers/ata/ata.h"
#include "string.h"
#include "vga.h"  // for debug printing

// Use static buffers to avoid stack overflow
static uint8_t sector_buf[512];
static uint8_t fat_buf[512];

// BIOS Parameter Block
static bpb_t bpb;
static uint32_t fat_start;
static uint32_t root_start;
static uint32_t data_start;

// --- Helpers ---

static void read_bpb(void) {
    ata_read_sector(0, sector_buf);

    // check boot signature 0x55AA
    if (sector_buf[510] != 0x55 || sector_buf[511] != 0xAA) {
        print("read_bpb: bad signature\n");
        return; // bail out safely
    }

    memcpy(&bpb, sector_buf + 11, sizeof(bpb));

    // quick sanity checks
    if (bpb.bytes_per_sector != 512) {
        print("BPB bytes/sector != 512: "); print_dec(bpb.bytes_per_sector);
        return;
    }
    if (bpb.sectors_per_fat == 0 || bpb.root_entries == 0) {
        print("\nBPB suspicious fields: ");
        print("\nspf="); print_dec(bpb.sectors_per_fat);
        print("\n root="); print_dec(bpb.root_entries);

        return;
    }

    fat_start  = bpb.reserved_sectors;
    root_start = fat_start + (bpb.num_fats * bpb.sectors_per_fat);
    data_start = root_start + ((bpb.root_entries * 32) / 512);

    print("\nBPB OK: fat_start="); print_dec(fat_start);
    print("\nroot start="); print_dec(root_start);
    print("\ndata_start="); print_dec(data_start);

}

static bool filename_match(dir_entry_t *entry, const char *name, const char *ext) {
    return (strncmp(entry->name, name, 8) == 0 &&
            strncmp(entry->ext, ext, 3) == 0);
}

// --- Public API ---


void fat16_init() {
    
    print("Initializing FAT16...\n");
    read_bpb();
    print("FAT16 ready\n");
    print("Initializing FAT16...\n");

    print("BPB values:\n");
    print("\nBytes per sector: "); print_dec(bpb.bytes_per_sector);
    print("\nSectors per cluster: "); print_dec(bpb.sectors_per_cluster);
    print("\nReserved sectors: "); print_dec(bpb.reserved_sectors);
    print("\nNum FATs: "); print_dec(bpb.num_fats);
    print("\nRoot entries: "); print_dec(bpb.root_entries);
    print("\nSectors per FAT: "); print_dec(bpb.sectors_per_fat);

    print("\nFAT16 ready\n");
}

// Reads a small file into out_buf; out_size will contain the file size
bool fat16_read_file(const char *name, const char *ext, uint8_t *out_buf, uint32_t *out_size) {
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
