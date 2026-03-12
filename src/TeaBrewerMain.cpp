#define BLYNK_TEMPLATE_ID "TMPL2POJe4R_F"
#define BLYNK_TEMPLATE_NAME "Assignment8"
#define BLYNK_AUTH_TOKEN "FuDSec8eU7LkFuEqI04rnYGMFlvKi2qA"

// WHAT WE WANT (DESIRED BEHAVIOR)
// Press button -> start cycle
// Cycle process:
// State IDLE until Start pressed
// State HEAT for 6 sec ("heating water")
// State STEEP for 6 sec ("steeping")
// State HOLD until switch closed
// State POUR for 4 sec
//
// Key interrupts:
// If switch opened while in POUR, go to HOLD
// If start button pressed at any time, go to IDLE
// 
// Additional:
// Slider selection for tea types, which have different brew times
// Time remaining display in Blynk

/**
 * PLANNED STRUCTURE:
 * Timer rasies TimerConcluded events
 * SwitchACtor raises CupPlaced and CupRemoved events
 * Blynk macro defined action raises ButtonPressed events
 */

// libraries
#include "Particle.h"
#include "blynk.h"


// my own code
#include "Event.h"
#include "BrewerState.h"
#include "BrewTimer.h"
#include "SwitchActor.h"
#include "IntervalActor.h"
#include "VPins.h"
#include "music/Music.h"

// core setup
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

const int SWITCH_PIN = D1; // put actual pin when wired
const int BUZZER_PIN = A2; // put actual pin when wired


// Key system states
BrewerState currState = IDLE;
boolean cupInBrewer = false;

// key selection state
boolean musicEnabled = false;

const String STATE_STRINGS[] = {"Idle", "Heat", "Steep", "Hold", "Pour"};



const uint32_t HEAT_TIME = 6000; // ms
uint32_t steepTime = 3000;
const uint32_t POUR_TIME = 4000;




BrewTimer timer([]() {handleEvent(TIMER_CONCLUDED);});
IntervalActor blynkTimerUpdater(1000, 0, []() {
    Blynk.virtualWrite(TIME_REMAINING_DISP, timer.getTimeLeft());
});

// TODO: Verify that this maps correctly to closing / opening
SwitchActor switchActor(SWITCH_PIN, []() {
    handleEvent(CUP_PLACED);
    Blynk.virtualWrite(CUP_STATUS_DISP, "Cup Placed");
    cupInBrewer = true;
}, []() {
    handleEvent(CUP_REMOVED);
    Blynk.virtualWrite(CUP_STATUS_DISP, "Cup Absent");
    cupInBrewer = false;
});


// Start button
BLYNK_WRITE(V0) {
    if (param.asInt() == 1) {
        handleEvent(BUTTON_PRESSED);
    }
}

// Tea type slider
BLYNK_WRITE(V5) {
    switch (param.asInt()) {
        case 0:
            steepTime = 3000;
        break;
        case 1:
            steepTime = 2000;
        break;
        case 2:
            steepTime = 4000;
        break;
    }
}

// music toggle
BLYNK_WRITE(V3) {
    if (param.asInt() == 1) {
        musicEnabled = true;
    } else {
        musicEnabled = false;
    }
}


void handleEvent(Event event) {
    // we conditionally handle events based on current state
    switch (event) {
        case BUTTON_PRESSED:
            if (currState == IDLE) {
                // if we're idle and start pressed, go to heat and start heat timer
                currState = HEAT;
                timer.startTimer(HEAT_TIME);
            } else {
                // If we're in any other state and stop pressed, go to idle
                currState = IDLE;
            }
        break;
        case CUP_PLACED:
            // we only care if the cup was placed while holding
            // if cup is placed while holding, start pouring
            if (currState == HOLD) {
                currState = POUR;
                timer.startTimer(POUR_TIME);
            }
        break;
        case CUP_REMOVED:
            // if cup is removed while pouring, return to hold
            if (currState == POUR) { 
                currState = HOLD;
            }
        break;
        case TIMER_CONCLUDED:
            switch (currState) {
                case HEAT:
                    // if timer concludes while heating, start steeping
                    currState = STEEP;
                    timer.startTimer(steepTime);
                break;
                case STEEP:
                    // if timer concludes while heating, check if cup is present.
                    // If cup is present, start pouring immediately, otherwise hold
                    if (cupInBrewer) {
                        currState = POUR;
                        timer.startTimer(POUR_TIME);
                    } else {
                        currState = HOLD;
                    }
                break;
                case POUR:
                    // if timer concludes while pouring, return to idle and play music if enabled
                    currState = IDLE;
                    if (musicEnabled) {
                        playTune(BUZZER_PIN);
                    }
                break;
            }
        break;
    }

    // finally, we want to update Blynk since our state just changed
    Blynk.virtualWrite(CURRENT_STATE_DISP, STATE_STRINGS[currState]);
}


void setup() {
    Serial.begin(9600);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop() {
    // do stuff
    timer.update();
    switchActor.act();
    Blynk.run();
    blynkTimerUpdater.act();
}