#include "../include/music/Tune.h"

Tune::Tune() {
    notes = std::vector<Note>();
}

void Tune::addNote(Note note) {
    notes.push_back(note);
}

void Tune::playTune(int pin) {
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].pitch == 0) {
            delay(notes[i].duration);
            continue;
        }

        switch (notes[i].articulation) {
            case STACCATO:
                tone(pin, notes[i].pitch, notes[i].duration * STACCATO_MULT);
            break;
            case NON_LEGATO:
                tone(pin, notes[i].pitch, notes[i].duration * NON_LEGATO_MULT);
            break;
            case LEGATO:
                tone(pin, notes[i].pitch, notes[i].duration * LEGATO_MULT);
            break;
        }
        delay(notes[i].duration);
    }
}
