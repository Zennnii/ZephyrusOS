#include "ata.h"

#define ATA_DATA      0x1F0
#define ATA_ERROR     0x1F1
#define ATA_SECCOUNT  0x1F2
#define ATA_LBA_LO    0x1F3
#define ATA_LBA_MID   0x1F4
#define ATA_LBA_HI    0x1F5
#define ATA_DRIVE     0x1F6
#define ATA_STATUS    0x1F7
#define ATA_CMD       0x1F7

#define ATA_CMD_READ  0x20

#define PIC1_COMMAND  0x20
#define PIC2_COMMAND  0xA0
#define PIC_EOI       0x20

// Wait until BSY clears
static void ata_wait_bsy() {
    while (inb(ATA_STATUS) & 0x80) ; // BSY = bit 7
}

// Wait until DRQ sets (data ready)
static void ata_wait_drq() {
    while (!(inb(ATA_STATUS) & 0x08)) ; // DRQ = bit 3
}

// Wait until BSY clears and check for errors
static int ata_wait_ready() {
    uint8_t status;
    do {
        status = inb(ATA_STATUS);
        if (status & 0x01) return -1; // ERR bit
    } while (status & 0x80); // BSY
    return 0; // OK
}

volatile uint16_t *ata_buffer = 0;
volatile int ata_ready = 0;

void ata_primary_isr(struct InterruptRegisters* regs) {
    uint8_t status = inb(ATA_STATUS);

    if (status & 0x08) { // DRQ set
        if (ata_buffer) {
            for (int i = 0; i < 256; i++)
                ata_buffer[i] = inw(ATA_DATA);
            ata_ready = 1;
        }
    }

    if (regs->int_no >= 40) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

int ata_read_sector(uint32_t lba, uint8_t *buffer) {
    ata_buffer = (uint16_t*)buffer;
    ata_ready = 0;

    // Wait until drive is ready
    if (ata_wait_ready() < 0) return -1; // drive error

    // Program LBA and sector count
    outb(ATA_SECCOUNT, 1);
    outb(ATA_LBA_LO,  lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HI,  (lba >> 16) & 0xFF);
    outb(ATA_DRIVE,   0xE0 | ((lba >> 24) & 0x0F));

    // Send READ command
    outb(ATA_CMD, ATA_CMD_READ);

    // Wait for ISR to set ata_ready, with timeout
    int timeout = 500000; // ~0.5s CPU cycles (adjust if needed)
    while (!ata_ready && --timeout) {
        __asm__ volatile("hlt");
    }

    if (!ata_ready) return -2; // timeout
    return 0; // success
}