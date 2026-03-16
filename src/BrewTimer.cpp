#include "../include/BrewTimer.h"

BrewTimer::BrewTimer(std::function<void()> timerEndCallback)
    : currentlyTiming(false), currentTimingInterval(0), timerStartTime(0), timerEndCallback(timerEndCallback) {}

void BrewTimer::startTimer(uint32_t time) {
    currentlyTiming = true;
    currentTimingInterval = time;
    timerStartTime = millis();
}

/**
 * Returns true if timer concluded, false if it's still running or there is
 * no timer currently running
 */
void BrewTimer::update() {
    if (currentlyTiming) {
        uint32_t currTime = millis();
        if (currTime - timerStartTime >= currentTimingInterval) {
            currentlyTiming = false;
            timerEndCallback();
        }
    }
}

uint32_t BrewTimer::getTimeLeft() {
    if (currentlyTiming) {
        return currentTimingInterval - (millis() - timerStartTime);
    } else {
        return 0;
    }
}

String BrewTimer::getFormattedTimeLeft() {
        uint32_t timeLeftms = currentTimingInterval - (millis() - timerStartTime);
        return String(timeLeftms / 1000) + " s";
}

boolean BrewTimer::getCurrentlyTiming() {
    return currentlyTiming;
}

void BrewTimer::stop() {
    currentlyTiming = false;
}