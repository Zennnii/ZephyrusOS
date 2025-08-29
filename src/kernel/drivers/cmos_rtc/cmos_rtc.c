#include "cmos_rtc.h"

static inline uint8_t cmos_read(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

static inline int rtc_update_in_progress(void) {
    outb(0x70, 0x0A);
    return inb(0x71) & 0x80;
}

static inline uint8_t bcd_to_bin(uint8_t val) {
    return (val & 0x0F) + ((val / 16) * 10);
}

typedef struct {
    int sec, min, hour, day, mon, year;
} rtc_time_t;

void read_rtc(rtc_time_t *t) {
    uint8_t regB;

    while (rtc_update_in_progress());

    t->sec = cmos_read(0x00);
    t->min = cmos_read(0x02);
    t->hour = cmos_read(0x04);
    t->day = cmos_read(0x07);
    t->mon = cmos_read(0x08);
    t->year = cmos_read(0x09);

    regB = cmos_read(0x0B);

    // Convert BCD to binary if needed
    if (!(regB & 0x04)) {
        t->sec = bcd_to_bin(t->sec);
        t->min = bcd_to_bin(t->min);
        t->hour = bcd_to_bin(t->hour);
        t->day = bcd_to_bin(t->day);
        t->mon = bcd_to_bin(t->mon);
        t->year = bcd_to_bin(t->year);
    }

    // Convert 12h to 24h if needed
    if (!(regB & 0x02) && (t->hour & 0x80)) {
        t->hour = ((t->hour & 0x7F) + 12) % 24;
    }
}

void print_date_time() {
    rtc_time_t now;
    read_rtc(&now);

    print("Time: ");
    print_dec(now.year); print("/");
    print_dec(now.mon);  print("/");
    print_dec(now.day);  print(" ");
    print_dec(now.hour); print(":");
    print_dec(now.min);  print(":");
    print_dec(now.sec);
    newLine();
}

void print_time_log() {
    rtc_time_t now;
    read_rtc(&now);

    print_dec(now.hour); print(":");
    print_dec(now.min); print(":");
    print_dec(now.sec);
}

void rtc_isr_handler() {
    outb(0x70, 0x0C);
    inb(0x71);
}

void init_rtc() {
    outb(0x70, 0x0C);
    inb(0x71);

    LOG_INFO("CMOS RTC Initialized Successfully");
}