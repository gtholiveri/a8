#ifndef SWITCHACTOR_H
#define SWITCHACTOR_H

#include <functional>

#include "Particle.h"

class SwitchActor {
   private:
    int pin;
    int32_t lastPoll;
    std::function<void()> risingAction;
    std::function<void()> fallingAction;

   public:
    SwitchActor(int pin, std::function<void()> risingAction, std::function<void()> fallingAction);
    void act();
};

#endif
