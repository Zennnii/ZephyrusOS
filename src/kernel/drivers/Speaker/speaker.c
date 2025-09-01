#include "speaker.h"

#define PIT_FREQUENCY 1193180

static int speaker_initialized = 0;

void init_speaker() {
    speaker_initialized = 1;
}

void speaker_play(uint32_t freq) {
    if (!speaker_initialized || freq == 0) return;

    // Limit frequency to avoid division by zero
    if (freq > PIT_FREQUENCY / 2) freq = PIT_FREQUENCY / 2;

    uint32_t divisor = PIT_FREQUENCY / freq;
    if (divisor == 0) divisor = 1;  // make sure divisor is at least 1

    // PIT channel 2, square wave
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(divisor & 0xFF));
    outb(0x42, (uint8_t)((divisor >> 8) & 0xFF));

    // Enable speaker
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 3);
}

void speaker_stop() {
    if (!speaker_initialized) return;

    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}