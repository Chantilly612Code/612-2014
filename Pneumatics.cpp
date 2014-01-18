#include <IterativeRobot.h>
#include "DriveTrain.h"
#include <DigitalInput.h>
#include <Relay.h>
#include "Pneumatics.h"
/*#include <EasterEgg.h>*/

void pneumatics::definePnuematics()
{
    switchObject = new DigitalInput(1, 1);
    compressor = new Relay(1, 1, Relay::kForwardOnly);
}

void pneumatics::checkPressure()
{
    if(switchObject->DigitalInput::Get() == 1)
    {
        compressor->Set(Relay::kForward);

    }

    if(switchObject->DigitalInput::Get() == 0)
    {
        compressor->Set(Relay::kOff);
    }
}
