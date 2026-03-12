#ifndef NOTE_H
#define NOTE_H

#include "Particle.h"
#include "music/Articulation.h"

class Note {
public:
    uint32_t duration;
    Articulation articulation;
    int pitch; // note: 0 is rest

    Note(uint32_t duration, Articulation articulation, int pitch) : duration(duration), articulation(articulation), pitch(pitch) {}
};

#endif
