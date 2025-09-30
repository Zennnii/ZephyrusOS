#ifndef ATA_PIO_H
#define ATA_PIO_H

#include "stdint.h"
#include "util/util.h"

// ATA I/O ports for primary bus, master drive
#define ATA_DATA       0x1F0
#define ATA_ERROR      0x1F1
#define ATA_SECCOUNT   0x1F2
#define ATA_LBA_LO     0x1F3
#define ATA_LBA_MID    0x1F4
#define ATA_LBA_HI     0x1F5
#define ATA_DRIVE      0x1F6
#define ATA_CMD        0x1F7
#define ATA_STATUS     0x1F7

#define ATA_CMD_READ   0x20
#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_DRQ 0x08

static void ata_wait_bsy();

static void ata_wait_drq();

static int ata_wait_ready();

int ata_read_sector(uint32_t lba, uint8_t *buffer);

void ata_primary_isr(struct InterruptRegisters* regs);

#endif