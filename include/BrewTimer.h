#ifndef BREWTIMER_H
#define BREWTIMER_H

#include <functional>

#include "Particle.h"

class BrewTimer {
   private:
    boolean currentlyTiming;
    uint32_t currentTimingInterval;
    uint32_t timerStartTime;
    std::function<void()> timerEndCallback;

   public:
    BrewTimer(std::function<void()> timerEndCallback);
    void startTimer(uint32_t time);
    void update();
    uint32_t getTimeLeft();
    String getFormattedTimeLeft();
    boolean getCurrentlyTiming();
    void stop();
};

#endif