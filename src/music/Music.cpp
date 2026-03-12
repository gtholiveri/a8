#include "Particle.h"
#include "Pitches.h"
#include "music/Tune.h"
#include "music/Note.h"


Tune getZeldaItemGetTune() {
    Tune melody;

    uint32_t sixteenthNoteDur = 150;
    
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_G5));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_FS5));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_DS5));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_A4));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_GS4));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_E5));
    melody.addNote(Note(sixteenthNoteDur, LEGATO, NOTE_GS5));
    melody.addNote(Note(800, LEGATO, NOTE_C6));

    return melody;
}

void playTune(int pin) {
    getZeldaItemGetTune().playTune(pin);
}