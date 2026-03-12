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

#include "Particle.h"
#include "blynk.h"

#include "Event.h"
#include "BrewerState.h"
#include "BrewTimer.h"
#include "SwitchActor.h"
#include "IntervalActor.h"
#include "VPins.h"
#include "music/Music.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);




const int SWITCH_PIN = D1; // put actual pin when wired
const int BUZZER_PIN = A2; // put actual pin when wired

BrewerState currState = IDLE;
BrewTimer timer([]() {handleEvent(TIMER_CONCLUDED);});
IntervalActor blynkUpdater(1000, 0, []() {
    Blynk.virtualWrite(TIME_REMAINING_DISP, timer.getTimeLeft());
    Blynk.virtualWrite(CURRENT_STATE_DISP, currState);
    Blynk.virtualWrite(CUP_STATUS_DISP, cupInBrewer);
});

// TODO: Verify that this maps correctly to closing / opening
SwitchActor switchActor(SWITCH_PIN, []() {handleEvent(CUP_PLACED);}, []() {handleEvent(CUP_REMOVED);});

boolean cupInBrewer = false;

const uint32_t HEAT_TIME = 6000; // ms
uint32_t steepTime = 3000;
const uint32_t POUR_TIME = 4000;

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


void handleEvent(Event event) {
    // we conditionally handle events based on current state
    switch (event) {
        case BUTTON_PRESSED:
            if (currState == IDLE) {
                // start the cycle from idle
                currState = HEAT;
                timer.startTimer(HEAT_TIME);
            } else {
                // wherever we are, just stop
                currState = IDLE;
            }
        break;
        case CUP_PLACED:
            // we only care if the cup was placed while holding
            if (currState == HOLD) {
                currState = POUR;
                timer.startTimer(POUR_TIME);
            }
            cupInBrewer = true;
        break;
        case CUP_REMOVED:
            cupInBrewer = false;
            if (currState == POUR) {
                currState = HOLD;
            }
        break;
        case TIMER_CONCLUDED:
            switch (currState) {
                case HEAT:
                    currState = STEEP;
                    timer.startTimer(steepTime);
                break;
                case STEEP:
                    if (cupInBrewer) {
                        currState = POUR;
                        timer.startTimer(POUR_TIME);
                    } else {
                        currState = HOLD;
                    }
                break;
                case POUR:
                    currState = IDLE;
                    playTune(BUZZER_PIN);
                break;
            }
        break;
    }
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
    blynkUpdater.act();
}