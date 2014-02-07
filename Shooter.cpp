#include "Shooter.h"
#include "612.h"
#include "main.h"
#include "SmoothJoystick.h"

const float Shooter::SPEED_AXISPOWER = 0.5f;

Shooter::Shooter(uint8_t axisMod,
                 uint8_t attractMod, uint32_t attractChan,
                 uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan,
                 uint32_t sjPort,
                 uint8_t bobModA, uint32_t bobChanA, uint8_t bobModB, uint32_t bobChanB)
{
    axis = new CANJaguar(axisMod);
    attractor = new Talon(attractMod, attractChan);
    clamper = new DoubleSolenoid(clampMod, clampFChan, clampRChan);
    shooterJoy = new SmoothJoystick(sjPort);
    robot -> gunnerJoy -> addJoyFunctions(&buttonHelper,(void*)this,PICKUP);
    //robot -> gunnerJoy -> addJoyFunctions(&buttonHelper,(void*)this,CLAMP_DOWN);
    robot -> update -> addFunctions(&updateHelper, (void*)this);
    bobTheEncoder = new Encoder(bobModA, bobChanA, bobModB, bobChanB);
    bobTheEncoder->Start();
    printf("Shooters have been updated\n");
    isPickingUp = false;
}

Shooter::~Shooter()
{
    delete axis;
    delete attractor;
    delete clamper;
}

void Shooter::pitchUp()
{
    axis->Set(SPEED_AXISPOWER);
}

void Shooter::pitchDown()
{
    axis->Set(-SPEED_AXISPOWER);
}

void Shooter::pitchStop()
{
    axis->Set(0);
}

void Shooter::pitchAngle(int32_t angle)
{
    originAng = currentAng;
    destinationAng = angle;
	if (angle > 0)
	{
		pitchUp();
		isPitchingUp = true;
	}
	if (angle < 0)
	{
		pitchDown();
		isPitchingDown = true;
	}
}

void Shooter::pull()
{
    attractor->Set(SPEED_ATTRACTOR);
}

void Shooter::pullStop()
{
    attractor->Set(0);
}

//@param goClamp moves clamper, off says to stop clamper
void Shooter::autoClamp()
{
    if(clamp == up)
    {
        clampDown();
    }
    else
    {
        clampUp();
    }
}

void Shooter::clampDown()
{
    pneumatics->setVectorValues(TIME, clamper, DoubleSolenoid::kForward);
    pull();
    clamp = down;
}

void Shooter::clampUp()
{
    pneumatics->setVectorValues(TIME, clamper, DoubleSolenoid::kReverse);
    pullStop();
    clamp = up;
}

//X to fire
void Shooter::buttonHelper(void* objPtr, uint32_t button){
    Shooter* shooterObj=(Shooter*)objPtr;
}

void Shooter::update()
{
    currentAng = bobTheEncoder->Get(); // TODO: CONVERT PULSE TO DEGREES
    if(shooterJoy -> GetTriggerState() == TRIG_L)
    {
        pitchUp();
    }
    else if(shooterJoy -> GetTriggerState() == TRIG_R)
    {
        pitchDown();
    }
    else
    {
        pitchStop();
    }
    if (isPitchingUp)
    {
		if (currentAng >= originAng + destinationAng)
		{
			pitchStop();
			isPitchingUp = false;
		}
    }
    if (isPitchingDown)
    {
		if (currentAng <= originAng + destinationAng)
		{
			pitchStop();
			isPitchingDown = false;
		}
    }
    if(shooterJoy -> GetSmoothButton(PICKUP))
    {
        if (!isPickingUp)
        {
            isPickingUp = true;
            pitchAngle(-23);
            clampDown();
        }
    }
    else
    {
        isPickingUp = false;
    	pitchAngle(23);
    	clampUp();
    }
}

void Shooter::updateHelper(void* instName)
{
    Shooter* shooterObj = (Shooter*)instName;
    shooterObj -> update();
}
