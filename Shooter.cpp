#include "Shooter.h"
#include "612.h"
#include "main.h"
#include "SmoothJoystick.h"

const float Shooter::SPEED_AXISPOWER = 0.5f;

Shooter::Shooter(uint8_t axisMod,
                 uint8_t attractMod, uint32_t attractChan,
                 uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan,
                 uint32_t sjPort,
                 int potSlot, int potChan, double potScale, double potOffset)
{
    axis = new CANJaguar(axisMod);
    attractor = new Talon(attractMod, attractChan);
    clamper = new DoubleSolenoid(clampMod, clampFChan, clampRChan);
    shooterJoy = new SmoothJoystick(sjPort);
    robot -> gunnerJoy -> addJoyFunctions(&buttonHelper,(void*)this,CLAMP_UP);
    robot -> gunnerJoy -> addJoyFunctions(&buttonHelper,(void*)this,CLAMP_DOWN);
    robot -> update -> addFunctions(&updateHelper, (void*)this);
    pot = new AnalogPotentiometer(potSlot, potChan, potScale, potOffset);
    printf("Shooters have been updated\n");
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

void Shooter::pitchAngle(double angle)
{
	if (angle > 0)
	{
		pitchUp();
		if (currentAng >= angle){
			pitchStop();
		}
	}
	if (angle < 0)
	{
		pitchDown();
		if (currentAng <= angle){
			pitchStop();
		}
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

//A for down, Y for clamp up, X to fire
void Shooter::buttonHelper(void* objPtr, uint32_t button){
    Shooter* shooterObj=(Shooter*)objPtr;
    if(button == CLAMP_UP)
    {
        shooterObj->clampUp();

    }
    else if(button == CLAMP_DOWN)
    {
        shooterObj->clampDown();
    }
}

void Shooter::update()
{
	currentAng = pot -> Get();
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

    if(shooterJoy -> GetSmoothButton(PICKUP))
    {
    	clampDown();
    	pitchAngle(-23);
    }
    else
    {
    	clampUp();
    	pitchAngle(65432);
    }
}

void Shooter::updateHelper(void* instName)
{
    Shooter* shooterObj = (Shooter*)instName;
    shooterObj -> update();
}
