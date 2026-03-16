#ifndef TUNE_H
#define TUNE_H

#include "Particle.h"
#include <vector>
#include "Note.h"

class Tune {
public:
    std::vector<Note> notes;

    const double STACCATO_MULT = 0.5;
    const double NON_LEGATO_MULT = 0.75;
    const double LEGATO_MULT = 1.0;

    Tune();
    void addNote(Note note);

    /**
     * WARNING this is blocking, I know I could do this non-blocking but like
     * Don't really want to right now and there's no point, the blocking behavior is kind of
     * desirable here honestly given there's no state for this
     */
    void playTune(int pin);
};

#endif
