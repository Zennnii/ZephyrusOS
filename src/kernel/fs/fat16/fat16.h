#ifndef FAT16_H
#define FAT16_H

#include "stdint.h"

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

/* Public API */
void fat16_init(void);
bool fat16_read_file(const char *name, const char *ext, uint8_t *out_buf, uint32_t *out_size);

#endif
