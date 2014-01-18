#include <IterativeRobot.h>
#include "DriveTrain.h"
#include <DigitalInput.h>
#include <Relay.h>
#include "Pneumatics.h"
#include <Timer.h>
#include <DoubleSolenoid.h>
#include <vector>
/*#include <EasterEgg.h>*/

using namespace std;

pneumatics pneumaticsObj;

void pneumaticsObj.definePneumaticsObjects(){

    switchObject = new DigitalInput(1, 1);
    compressor = new Relay(1, 1, Relay::kForwardOnly);
    solenoidTimer = new Timer();
    testSolenoid = new DoubleSolenoid();
}

void pneumaticsObj.checkPressure(){

    if(switchObject->DigitalInput::Get() == 1)
    {
        compressor->Set(Relay::kForward);

    }

    if(switchObject->DigitalInput::Get() == 0)
    {
        compressor->Set(Relay::kOff);
    }
}

void pneumaticsObj.updateSolenoid(){

    //This function checks if the solenoid has expired
    for(int i = 0; i < time.size(); i++){
        if(timerObject[i]->Get() >= time[i]){
            solenoid[i]->Set(DoubleSolenoid::kOff);
            //next, remove from vector
        }
    }
}

void pneumaticsObj.setVectorValues(double timerValues, DoubleSolenoid* startSolenoid, DoubleSolenoid::Value){

    solenoidTimer = new Timer();
    testSolenoid = new DoubleSolenoid();
    timerObject.push_back(timerValues);
    solenoid.push_back(startSolenoid);
    testSolenoid->Set(DoubleSolenoid::Value = kOff);
}
