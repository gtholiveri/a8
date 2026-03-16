#ifndef MUSIC_H
#define MUSIC_H

#include "Particle.h"
#include "Tune.h"
#include "Note.h"
#include "Pitches.h"


extern const uint32_t NOTE_DUR;

Tune getZeldaItemGetTune();
void playTune(int pin);
void playLongerTune(int pin);
#endif
