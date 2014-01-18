#ifndef PNEUMATICS_H
#define PNEUMATICS_H

#include <IterativeRobot.h>
#include "DriveTrain.h"
#include <DigitalInput.h>
#include <Relay.h>
#include <Timer.h>
#include <DoubleSolenoid.h>

class pneumatics
{
    public:
        void definePnuematicsObjects();
        void checkPressure();
        void setVectorValues(double timerValues, DoubleSolenoid* startSolenoid, DoubleSolenoid::Value);
        void updateSolenoid();
    private:
        DigitalInput* switchObject;
        Relay* compressor;
        vector <double> time;
        vector <Timer*> timerObject
        vector <DoubleSolenoid*> solenoid;
        DoubleSolenoid* testSolenoid;

};
#endif // PNEUMATICS_H
