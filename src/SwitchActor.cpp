#include "../include/SwitchActor.h"

SwitchActor::SwitchActor(int pin,
                         std::function<void()> risingAction,
                         std::function<void()> fallingAction)
    : pin(pin),
      lastPoll(digitalRead(pin)),
      risingAction(risingAction),
      fallingAction(fallingAction) {}

void SwitchActor::act() {
    int32_t currPoll = digitalRead(pin);
    if (lastPoll == HIGH && currPoll == LOW) {
        fallingAction();
    } else if (lastPoll == LOW && currPoll == HIGH) {
        risingAction();
    }
    lastPoll = currPoll;
}