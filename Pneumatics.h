#ifndef PNEUMATICS_H
#define PNEUMATICS_H

#include <IterativeRobot.h>
#include "DriveTrain.h"
#include <DigitalInput.h>
#include <Relay.h>
#include <Timer.h>
#include <DoubleSolenoid.h>
#include <vector>

using namespace std;

class pneumatics{

    public:
        void definePneumaticsObjects();
        void checkPressure();
        void setVectorValues(double timerValues, DoubleSolenoid* startSolenoid, DoubleSolenoid::Value);
        void updateSolenoid();
    private:
        DigitalInput* switchObject;
        Relay* compressor;
        Timer* solenoidTimer;
        vector <double> time;
        vector <Timer*> timerObject;
        vector <DoubleSolenoid*> solenoid;
        DoubleSolenoid* testSolenoid;

};
#endif // PNEUMATICS_H
