#ifndef PNEUMATICS_H
#define PNEUMATICS_H

#include <IterativeRobot.h>
#include "DriveTrain.h"
#include <DigitalInput.h>
#include <Relay.h>

class pneumatics
{
    public:
        void definePnuematics();
        void checkPressure();
    private:
        DigitalInput* switchObject;
        Relay* compressor;
};
#endif // PNEUMATICS_H
