#include "ata.h"

bool ata_drive_present = false;

// Wait until BSY clears
static void ata_wait_bsy() {
    while (inb(ATA_STATUS) & ATA_SR_BSY) ; // BSY = bit 7
}

// Wait until DRQ sets (data ready)
static void ata_wait_drq() {
    while (!(inb(ATA_STATUS) & ATA_SR_DRQ)) ; // DRQ = bit 3
}

// Wait until BSY clears and check for errors
static int ata_wait_ready() {
    uint8_t status;
    do {
        status = inb(ATA_STATUS);
        if (status & ATA_SR_ERR) return -1; // ERR bit
    } while (status & ATA_SR_BSY); // BSY
    return 0; // OK
}

bool ata_detect_drive(void) {
    outb(ATA_DRIVE, 0xA0); // master
    io_wait();

    outb(ATA_SECCOUNT, 0);
    outb(ATA_LBA_LO, 0);
    outb(ATA_LBA_MID, 0);
    outb(ATA_LBA_HI, 0);

    uint8_t status = inb(ATA_STATUS);

    if (status == 0) {
        return false; // definitely no drive
    }

    // Wait for BSY to clear
    while (status & ATA_SR_BSY) {
        status = inb(ATA_STATUS);
    }

    uint8_t cl = inb(ATA_LBA_MID);
    uint8_t ch = inb(ATA_LBA_HI);

    // ATA drive signature
    if (cl == 0x00 && ch == 0x00) {
        return true; // ATA drive present
    }

    // ATAPI drive signature
    if (cl == 0x14 && ch == 0xEB) {
        return true; // ATAPI (CD/DVD)
    }

    return false;
}

void ata_init(void) {
    bool drive_present = ata_detect_drive();

    ata_drive_present = drive_present;

    if (!drive_present) {
        LOG_INFO("No ATA drive detected. Running in live mode.");
    } else {
        LOG_INFO("ATA drive detected.");
    }
}

volatile uint16_t *ata_buffer = 0;
volatile int ata_ready = 0;
volatile const uint16_t *ata_wbuffer = 0;  // write buffer
volatile int ata_done = 0;                 // set by ISR when operation completes
volatile int ata_mode = 0;                 // 0 = read, 1 = write

void ata_primary_isr(struct InterruptRegisters* regs) {
    uint8_t status = inb(ATA_STATUS);
    
    if (ata_mode == 0) { // READ mode
        if (status & 0x08) { // DRQ
            if (ata_buffer) {
                for (int i = 0; i < 256; i++)
                    ata_buffer[i] = inw(ATA_DATA);
                ata_done = 1;
                ata_ready = 1;
            }
        }
    }

    if (regs->int_no >= 40) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

int ata_read_sector(uint32_t lba, uint8_t *buffer) {
    if (!ata_drive_present) return -1; // Skip reads if no drive

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

int ata_write_sector(uint32_t lba, const uint8_t *buffer) {
    // Cast buffer to 16-bit words (ATA PIO requires 256 words per sector)
    const uint16_t *buf16 = (const uint16_t*)buffer;
    
    // Wait until drive is ready
    if (ata_wait_ready() < 0) return -1; // drive error

    outb(ATA_SECCOUNT, 1); // write 1 sector
    outb(ATA_LBA_LO,  lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HI,  (lba >> 16) & 0xFF);
    outb(ATA_DRIVE,   0xE0 | ((lba >> 24) & 0x0F));

    // Send WRITE SECTOR command
    outb(ATA_CMD, ATA_CMD_WRITE);

    // Wait until drive requests data (DRQ = 1)
    ata_wait_bsy();
    ata_wait_drq();

    // Transfer 256 words (512 bytes)
    for (int i = 0; i < 256; i++) {
        outw(ATA_DATA, buf16[i]);
    }

    if (ata_wait_ready() < 0) return -2;

    return 0;
}