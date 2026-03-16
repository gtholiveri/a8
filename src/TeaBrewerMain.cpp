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
#include"../include/Event.h"
#include"../include/BrewerState.h"
#include"../include/BrewTimer.h"
#include"../include/SwitchActor.h"
#include"../include/IntervalActor.h"
#include"../include/VPins.h"
#include"../include/music/Music.h"
#include"../include/music/Pitches.h"
#include"../include/TeaType.h"


// core setup
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_WARN);

const int SWITCH_PIN = D2; // put actual pin when wired
const int BUZZER_PIN = A2; // put actual pin when wired


// Key system states
BrewerState currState = IDLE;
TeaType currTeaType = BLACK;
boolean cupInBrewer = false;

// key selection state
boolean musicEnabled = false;

const String STATE_NAMES[] = {"Idle", "Heat", "Steep", "Hold", "Pour"};
const String TEA_TYPE_NAMES[] {"Black Tea", "Green Tea", "Herbal Tea"};
const uint32_t STEEP_TIMES[] {3000, 2000, 4000};


const uint32_t HEAT_TIME = 6000; // ms
uint32_t steepTime = 3000;
const uint32_t POUR_TIME = 4000;


void handleEvent(Event event);

BrewTimer timer([]() {handleEvent(TIMER_CONCLUDED);});

uint32_t getTotalRemainingTime();
IntervalActor blynkTimerUpdater(1000, 0, []() {
    if (!cupInBrewer && currState == HOLD) {
        Blynk.virtualWrite(STEP_TIME_REMAINING_DISP, "Waiting for cup.");
    } else if (currState == IDLE) {
        Blynk.virtualWrite(STEP_TIME_REMAINING_DISP, "Waiting to begin brewing.");
        Blynk.virtualWrite(TOTAL_TIME_REMAINING_DISP, "Waiting to begin brewing.");

    } else if (timer.getCurrentlyTiming()) {
        Blynk.virtualWrite(STEP_TIME_REMAINING_DISP, timer.getFormattedTimeLeft());
    }
    if (currState != IDLE) Blynk.virtualWrite(TOTAL_TIME_REMAINING_DISP, String(getTotalRemainingTime() / 1000) + " s");
});

// TODO: Verify that this maps correctly to closing / opening
SwitchActor switchActor(SWITCH_PIN, []() {
    handleEvent(CUP_REMOVED);
    Blynk.virtualWrite(CUP_STATUS_DISP, "Cup Absent");
    cupInBrewer = false;
}, []() {
    handleEvent(CUP_PLACED);
    Blynk.virtualWrite(CUP_STATUS_DISP, "Cup Placed");
    cupInBrewer = true;
});


// Start button
BLYNK_WRITE(V0) {
    if (param.asInt() == 1) {
        handleEvent(BUTTON_PRESSED);
    }
}

// Tea type slider
BLYNK_WRITE(V5) {
    int teaInd = param.asInt();
    currTeaType = (TeaType) teaInd;
    steepTime = STEEP_TIMES[teaInd];
    Serial.println("R: " + TEA_TYPE_NAMES[teaInd] + " selected");
}

// music toggle
BLYNK_WRITE(V3) {
    if (param.asInt() == 1) {
        musicEnabled = true;

        Serial.println("R: Music enabled");
    } else {
        musicEnabled = false;

        Serial.println("R: Music disabled");
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

                Serial.println("A: Heating for " + String(HEAT_TIME));
            } else {
                // If we're in any other state and stop pressed, go to idle
                currState = IDLE;
                timer.stop();

                Serial.println("A: Cycle canceled");
            }
        break;
        case CUP_PLACED:
            // we only care if the cup was placed while holding
            // if cup is placed while holding, start pouring
            if (currState == HOLD) {
                currState = POUR;
                timer.startTimer(POUR_TIME);
                Serial.println("A: Pouring");
            }
        break;
        case CUP_REMOVED:
            // if cup is removed while pouring, return to hold
            if (currState == POUR) { 
                currState = HOLD;
                timer.stop();
                Serial.println("A: Pour held since cup removed");
            }
        break;
        case TIMER_CONCLUDED:
            switch (currState) {
                case HEAT:
                    // if timer concludes while heating, start steeping
                    currState = STEEP;
                    timer.startTimer(steepTime);
                    Serial.println("A: Steeping for " + TEA_TYPE_NAMES[currTeaType] + ", " + String(STEEP_TIMES[currTeaType]) + " ms");
                break;
                case STEEP:
                    // if timer concludes while heating, check if cup is present.
                    // If cup is present, start pouring immediately, otherwise hold
                    if (cupInBrewer) {
                        currState = POUR;
                        timer.startTimer(POUR_TIME);
                        Serial.println("A: Pouring immediately");
                    } else {
                        currState = HOLD;
                        Serial.println("A: Holding for cup.");
                    }
                break;
                case POUR:
                    // if timer concludes while pouring, return to idle and play music if enabled
                    currState = IDLE;
                    if (musicEnabled) {
                        Serial.println("A: Playing tune");
                        playTune(BUZZER_PIN);
                    }
                break;
            }
        break;
    }

    // finally, we want to update Blynk since our state just changed
    Blynk.virtualWrite(CURRENT_STATE_DISP, STATE_NAMES[currState]);
}


void setup() {
    Serial.begin(9600);
    pinMode(SWITCH_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN);
}




uint32_t getTotalRemainingTime() {
    uint32_t timeRemaining = timer.getCurrentlyTiming() ? timer.getTimeLeft() : 0;// ms
    for (int i = currState + 1; i <= POUR; i++) {
        switch (i) {
            case HEAT:
                timeRemaining += HEAT_TIME;
            break;
            case STEEP:
                timeRemaining += STEEP_TIMES[currTeaType];
            break;
            case POUR:
                timeRemaining += POUR_TIME;
            break;
        }
    }
    return timeRemaining;
}

boolean buzzed = false;
SwitchActor testSwitch(SWITCH_PIN, []() {Serial.println("Switch opened");}, []() {Serial.println("Switch closed");});
IntervalActor testBuzzer(BUZZER_PIN, 10000, []() {if (!buzzed) {tone(BUZZER_PIN, NOTE_C4, 100); buzzed = true;}});

void test() {
    testSwitch.act();
    testBuzzer.act();
}


void loop() {
    // do stuff
    timer.update();
    switchActor.act();
    Blynk.run();
    blynkTimerUpdater.act();

    // test();    
}