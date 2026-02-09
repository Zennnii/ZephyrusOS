#ifndef ATA_H
#define ATA_H
#include "stdint.h"
#include "util/util.h"

// ATA I/O ports for primary bus, master drive
#define ATA_DATA      0x1F0
#define ATA_ERROR     0x1F1
#define ATA_SECCOUNT  0x1F2
#define ATA_LBA_LO    0x1F3
#define ATA_LBA_MID   0x1F4
#define ATA_LBA_HI    0x1F5
#define ATA_DRIVE     0x1F6
#define ATA_STATUS    0x1F7
#define ATA_CMD       0x1F7

#define ATA_SR_BSY    0x80  // Busy
#define ATA_SR_DRDY   0x40  // Drive ready
#define ATA_SR_DF     0x20  // Drive fault
#define ATA_SR_DSC    0x10
#define ATA_SR_DRQ    0x08  // Data request
#define ATA_SR_ERR    0x01  // Error

#define ATA_CMD_READ  0x20

#define ATA_CMD_WRITE 0x30

#define PIC1_COMMAND  0x20
#define PIC2_COMMAND  0xA0
#define PIC_EOI       0x20

extern bool ata_drive_present;

bool ata_detect_drive();

void ata_init(void);

int ata_read_sector(uint32_t lba, uint8_t *buffer);

void ata_primary_isr(struct InterruptRegisters* regs);

#endif