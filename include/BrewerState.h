#ifndef BREWERSTATES_H
#define BREWERSTATES_H
#include <map>
#include "Particle.h"

enum BrewerState {
    IDLE,
    HEAT,
    STEEP,
    HOLD,
    POUR
};
#endif