#ifndef BREWTIMER_H
#define BREWTIMER_H

#include "Particle.h"
#include <functional>

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
};

#endif