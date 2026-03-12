#ifndef TUNE_H
#define TUNE_H

#include "Particle.h"
#include <vector>
#include "music/Note.h"

class Tune {
public:
    std::vector<Note> notes;

    const double STACCATO_MULT = 0.5;
    const double NON_LEGATO_MULT = 0.75;
    const double LEGATO_MULT = 1.0;

    Tune() {
        notes = std::vector<Note>();
    }

    void addNote(Note note) {
        notes.push_back(note);
    }

    /**
     * WARNING this is blocking, I know I could do this non-blocking but like
     * Don't really want to right now and there's no point, the blocking behavior is kind of
     * desirable here honestly given there's no state for this
     */
    void playTune(int pin) {
        for (int i = 0; i < notes.size(); i++) {
            if (notes[i].pitch == 0) {
                delay(notes[i].duration);
                continue;
            }

            switch (notes[i].articulation) {
                case STACCATO:
                    tone(notes[i].pitch, notes[i].duration * STACCATO_MULT);
                break;
                case NON_LEGATO:
                    tone(notes[i].pitch, notes[i].duration * NON_LEGATO_MULT);
                break;
                case LEGATO:
                    tone(notes[i].pitch, notes[i].duration * LEGATO_MULT);
                break;
            }
            delay(notes[i].duration);
        }
    }
};

#endif
