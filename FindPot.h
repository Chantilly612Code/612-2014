#ifndef FINDPOT_H
#define FINDPOT_H

#include <AnalogPotentiometer.h>
#include "Ports.h"

class findPot(int POT_SLOT, int POT_CHANNEL, double POT_SCALE, double POT_OFFSET)
{
public:
    findPot::findPot();
    findPot::~findPot();
    
    double grab();
    double convertAngle();
};

#endif
