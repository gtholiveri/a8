#ifndef MUSIC_H
#define MUSIC_H

#include "Particle.h"
#include "music/Tune.h"
#include "music/Note.h"

extern const uint32_t NOTE_DUR;

Tune getZeldaItemGetTune();
void playTune(int pin);

#endif
