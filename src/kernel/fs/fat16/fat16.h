#ifndef FAT16_H
#define FAT16_H

#include "stdint.h"
#include "fb/dis.h"
#include "fb/colors.h"

#define SECTOR_SIZE 512

/* BPB and dir_entry definitions (packed) */
typedef struct __attribute__((packed)) {
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_short;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat;
} bpb_t;

typedef struct __attribute__((packed)) {
    char     name[8];
    char     ext[3];
    uint8_t  attr;
    uint8_t  reserved[10];
    uint16_t time;
    uint16_t date;
    uint16_t first_cluster;
    uint32_t file_size;
} dir_entry_t;

#pragma pack(push, 1)
typedef struct {
    uint8_t status;        // 0x00: bootable flag (0x80 = bootable)
    uint8_t chs_first[3];  // 0x01: CHS address of first sector
    uint8_t type;          // 0x04: partition type
    uint8_t chs_last[3];   // 0x05: CHS address of last sector
    uint32_t lba_first;    // 0x08: starting LBA
    uint32_t sectors;      // 0x0C: number of sectors
} mbr_partition_t;
#pragma pack(pop)

/* Public API */
bool fat16_init(void);
bool fat16_read_file(const char *name, const char *ext, uint8_t *out_buf, uint32_t *out_size);

#endif
