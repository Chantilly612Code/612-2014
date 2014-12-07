#include "FindPot.h"

findPot::findPot(int POT_SLOT, int POT_CHANNEL, double POT_SCALE, double POT_OFFSET)
{
    AnalogPotentiometer* anglePot = new AnalogPotentiometer(POT_SLOT, POT_CHANNEL, POT_SCALE, POT_OFFSET);
}

findPot::~findPot()
{
  
}

double findPot::grab()
{
  
}

double findPot::convertAngle(findPot::grab())
{
  
}
