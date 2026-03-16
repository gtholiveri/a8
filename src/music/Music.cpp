#include "../include/music/Music.h"



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

Tune getZeldaTreasureOpenTune() {
    Tune t;

    int bpmi = 80;
    int bpmf = 160;

    int currSixteenthDur;

    int sequence[] = {
    NOTE_G3, NOTE_A3, NOTE_B3, NOTE_CS4,
    NOTE_G3, NOTE_A3, NOTE_B3, NOTE_CS4,
    NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_D4,
    NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_D4,
    NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_DS4,
    NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_DS4,
    NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_E4,
    NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_E4,

    NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_F4,
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_FS4,
    NOTE_CS4, NOTE_DS4, NOTE_F4, NOTE_G4,
    NOTE_D4, NOTE_E4, NOTE_FS4, NOTE_GS4
    };

    int n = sizeof(sequence) / sizeof(sequence[0]);
    for (int i = 0; i < n; i++) {
        double bpmcurr = bpmi + (bpmf - bpmi) * (double)i / (n - 1); // linearly interpolate current bpm
        currSixteenthDur = 60000.0 / bpmcurr / 4.0; // we end up just rounding this
        t.addNote(Note(currSixteenthDur, LEGATO, sequence[i]));
    }

    int halfDur = currSixteenthDur * 8;
    int quarterDur = currSixteenthDur * 4;

    t.addNote(Note(4 * quarterDur, LEGATO, 0));
    t.addNote(Note(quarterDur, LEGATO, NOTE_A5));
    t.addNote(Note(quarterDur, LEGATO, NOTE_AS5));
    t.addNote(Note(quarterDur, LEGATO, NOTE_B5));
    t.addNote(Note(3 * quarterDur, LEGATO, NOTE_C6));

    return t;
}

void playTune(int pin) {
    static Tune tune = getZeldaItemGetTune();
    tune.playTune(pin);
}

void playLongerTune(int pin) {
    static Tune longerTune = getZeldaTreasureOpenTune();
    longerTune.playTune(pin);
}