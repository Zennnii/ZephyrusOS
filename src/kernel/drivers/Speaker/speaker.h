#ifndef SPEAKER_H
#define SPEAKER_H

#include "stdint.h"
#include "util/util.h"

void init_speaker();
void speaker_play(uint32_t freq);
void speaker_stop();

#endif