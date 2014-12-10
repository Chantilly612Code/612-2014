#include "Shooter.h"
#include "612.h"
#include "SmoothJoystick.h"
#include "main.h"
#include "ADXL345_I2C_612.h"

const double Shooter::SPEED_AXISPOWER_TELEOP = 0.80;
const double Shooter::SPEED_AXISPOWER_AUTO_SLOW = 0.40;
const double Shooter::SPEED_AXISPOWER_AUTO_VERY_SLOW = 0.25;
const double Shooter::SPEED_AXISPOWER_AUTO_FAST = 0.80;
const double Shooter::SPEED_ATTRACTOR = 1.0;
const double Shooter::SPEED_WORM = 1.0;

Shooter::Shooter(main_robot* r,uint8_t axisCan,
                 uint8_t attractMod, uint32_t attractChan,
                 uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan,
                 uint8_t wormCan,
                 uint8_t punchMod,uint32_t punchFChan,uint32_t punchRChan,
                 uint8_t bobMod,
				 Pneumatics* pnum7Wheel, DoubleSolenoid* solenoid7Wheel)
                 :isPickingUp(false),isPitchingUp(false),
                 isPitchingDown(false),wormIsPulling(false),winching(false),
                 hasTilted(false),isPickingUpStopping(false),autoPulling(false),
                 smartFiring(false),accelWorking(true),smartFireTimer(new Timer())
{
    robot = r;
    pnum_7Wheel = pnum7Wheel;
	solenoid_7Wheel = solenoid7Wheel;
    axis = new CANJaguar(axisCan);
    attractor = new Talon(attractMod, attractChan);
    clamper = new DoubleSolenoid(clampMod, clampFChan, clampRChan);
    wormGear = new CANJaguar(wormCan);
    puncher = new DoubleSolenoid(punchMod,punchFChan,punchRChan);
    bobTheAccelerometer = new ADXL345_I2C_612(bobMod);
//  bobThePot = new AnalogChannel(1,5);
    isPickingUp = false;
    //7WheelExtend = false;
    //7WheelOldState = false;
    shooterJoy = robot -> gunnerJoy;
    shooterJoy -> addJoyFunctions(&buttonHelper,(void*)this,CLAMP);
    shooterJoy -> addJoyFunctions(&buttonHelper,(void*)this,ENERGIZE);
    shooterJoy -> addJoyFunctions(&buttonHelper,(void*)this,FIRE);
    shooterJoy -> addJoyFunctions(&buttonHelper,(void*)this,AUTO_HIGHGOAL);
    shooterJoy -> addJoyFunctions(&buttonHelper,(void*)this,AUTO_VERTICAL);
    shooterJoy -> addJoyFunctions(&setPickupHelper,(void*)this,PICKUP);
    robot -> update -> addFunctions(&updateHelper, (void*)this);
    smartFireTimer->Stop();
}

Shooter::~Shooter()
{
    delete axis;
    delete pnum_7Wheel;
    delete attractor;
    delete clamper;
    delete bobTheAccelerometer;
    delete wormGear;
    delete puncher;
}

double Shooter::getPitchSpeed()
{
    if(isPitchingUp || isPitchingDown)
    {
        if(fabs(originPitch - destinationPitch) > AXIS_SPEED_FAR_THRESH)
        {
            return SPEED_AXISPOWER_AUTO_FAST;
        }
        else
        {
            if(fabs(currentPitch - destinationPitch) < AXIS_SPEED_CLOSE_THRESH)
            {
                return SPEED_AXISPOWER_AUTO_VERY_SLOW;
            }
            else
            {
                return SPEED_AXISPOWER_AUTO_SLOW;
            }
        }
    }
    else
    {
        return SPEED_AXISPOWER_TELEOP;
    }
}

void Shooter::pitchUp()
{
    axis->Set(-getPitchSpeed());
}


void Shooter::pitchDown()
{
    axis->Set(getPitchSpeed());
}

void Shooter::pitchStop()
{
    isPitchingUp = false;
    isPitchingDown = false;
    axis->Set(0);
}

void Shooter::pitchAngle(double newPitch)
{
    if(!accelWorking)
    {
        return;
    }
    originPitch = currentPitch;
    destinationPitch = newPitch;
    isPickingUp = false;
    isPitchingDown = false;
    hasTilted = false;
    if (newPitch < originPitch)
    {
        isPitchingDown = true;
    }
    if (newPitch > originPitch)
    {
        isPitchingUp = true;
    }
}

void Shooter::rollerPull()
{
    attractor->Set(SPEED_ATTRACTOR); // apparently its supposed to be negative to pull
}

void Shooter::rollerStop()
{
    attractor->Set(0);
}

void Shooter::rollerRepel()
{
    attractor->Set(-SPEED_ATTRACTOR);
}

//@param goClamp moves clamper, off says to stop clamper
void Shooter::autoClamp()
{
    if(clamp == up)
        clampDown();
    else
        clampUp();
}

void Shooter::clampDown()
{
    robot -> pnum->setVectorValues(TIME, clamper, DoubleSolenoid::kForward);
    clamp = down;
}

void Shooter::clampUp()
{
    robot -> pnum->setVectorValues(TIME, clamper, DoubleSolenoid::kReverse);
    clamp = up;
}

void Shooter::auto7Wheel() 
{
	/*if(!(ShooterJoy -> GetSmoothButton(CLAMP))
	{
	    7WheelOldState = false;
	}
	else
	{
	    if(!7WheelOldState)
	    {
	        7WheelOldState = true;
	        7WheelExtend = !7WheelExtend;
	    }
	}
	if(7WheelExtend)
	{
	    pnum_7Wheel->setVectorValues(TIME, solenoid_7Wheel, DoubleSolenoid::kForward);
	}
	else
	{
	    pnum_7Wheel->setVectorValues(TIME, solenoid_7Wheel, DoubleSolenoid::kReverse);
	}*/
	if(sevenWheel == up)
	{
        sevenWheelDown();
    }
    else
    {
        sevenWheelUp();
    }
}
void Shooter::sevenWheelDown()
{
    robot -> pnum->setVectorValues(TIME, solenoid_7Wheel, DoubleSolenoid::kForward);
    sevenWheel = down;
}

void Shooter::sevenWheelUp()
{
    robot -> pnum->setVectorValues(TIME, solenoid_7Wheel, DoubleSolenoid::kReverse);
    sevenWheel = up;
}

void Shooter::wormStop()
{
    wormGear -> Set(0);
    wormIsPulling = false;
    //currentSpeed = SPEED_WORM;
}

bool Shooter::wormDone()
{
    if (shooterJoy->GetSmoothButton(BUTTON_Y))
    {
        std::printf("Worm Drive OFF\n");
        return true;
    }
    else
    {
        bool off = !(wormGear -> GetForwardLimitOK());
        if (off == true)
        {
            std::printf("Worm Drive OFF\n");
        }
        return off;
    }
}
 
void Shooter::punch()
{
    robot -> pnum -> setVectorValues(PUNCH_TIME, puncher, DoubleSolenoid::kReverse);
}

void Shooter::smartFire()
{
    if(!smartFiring)
    {
        clampUp();
        smartFiring = true;
        smartFireTimer->Reset();
        smartFireTimer->Start();
    }
}

bool Shooter::doubleEqual(double a,double b)
{
    return fabs(a-b) < FLOAT_THRESH;
}

void Shooter::buttonHelper(void* objPtr, uint32_t button)
{
    Shooter* shooterObj=(Shooter*)objPtr;
    if(button==CLAMP)
    {
        shooterObj->autoClamp();
        shooterObj->auto7Wheel();//Not actually sure what button this should go on. Feel free to move this method
        // if you arent happy. I dont think the clamp actually does anthing, however, so it should be fine.
    }
    if(button==ENERGIZE)
    {
        shooterObj->wormPull();
    }
    if(button==FIRE)
    {
        shooterObj->smartFire();
    }
    if(button==AUTO_HIGHGOAL)
    {
        shooterObj->pitchAngle(HIGHGOAL_POSITION);
    }
    if(button==AUTO_VERTICAL)
    {
        shooterObj->pitchAngle(VERTICAL_POSITION);
    }
}

double Shooter::getAngle() {
    double bobX = bobTheAccelerometer->GetAcceleration(ADXL345_I2C_612::kAxis_X);
    double bobY = bobTheAccelerometer->GetAcceleration(ADXL345_I2C_612::kAxis_Y);
    double bobZ = bobTheAccelerometer->GetAcceleration(ADXL345_I2C_612::kAxis_Z);
    accelWorking = !(doubleEqual(bobX,0.0) && doubleEqual(bobY,0.0) && doubleEqual(bobZ,0.0));
    if(!accelWorking)
    {
        isPitchingUp = false;
        isPitchingDown = false;
    }
    double newPitch = (atan2(bobX, sqrt(bobY*bobY + bobZ*bobZ))*180.0)/PI;
    //    if(fabs(newPitch-currentPitch) < 10)
    currentPitch = newPitch;
/*  double volts = bobThePot->GetVoltage();
    currentPitch = bobThePot; // TODO get pot conversion */
    return currentPitch;
}

/*
 * 
 * The following code is a shortcut of sorts for Ben so that he can press one button to do a bunch of stuff
 * 
 */ 

void Shooter::setPickup() {
    clampDown();
    pitchAngle(PICKUP_POSITION);
}

void Shooter::setPickupHelper(void* instName, uint32_t button) {
    Shooter* shooterObj = (Shooter*)instName;
    shooterObj -> setPickup();
}


// TODO IMPORTANT: What if we are pitching down to pickup angle, but we never reach
//                 there because we hit the frame? Somehow we need to detect that
//                 and set isPitchingDown to false if it hard stops

void Shooter::update()
{
    getAngle();

    static int output = 0;
    if(output%20 == 0)
    {
        printf("Tilt Angle: %f\n",currentPitch);
    }
    output++;

    // angle presets, triggers
    if(shooterJoy -> GetTriggerState() == AUTO_LOWGOAL && !doubleEqual(destinationPitch, LOWGOAL_POSITION))
    {
        pitchAngle(LOWGOAL_POSITION);
    }
    else if(shooterJoy -> GetTriggerState() == AUTO_PICKUP && !doubleEqual(destinationPitch, PICKUP_POSITION))
    {
        pitchAngle(PICKUP_POSITION);
    }

    // manual controls
    if(!isPitchingUp && !isPitchingDown)
    {
        if(shooterJoy -> IsAxisZero(TILT))
            pitchStop();
        else if(shooterJoy -> GetRawAxis(TILT) < 0) // push up = negative values = tilt down
            pitchDown();
        else
            pitchUp();
    }

    if(shooterJoy -> IsAxisZero(ROLLERS))
        rollerStop();
    else if(shooterJoy -> GetRawAxis(ROLLERS) < 0) // push down = pull, push up = repel
        rollerPull();
    else
        rollerRepel();

    if (isPitchingUp)
    {
        pitchUp();
        if (currentPitch >= destinationPitch || !(axis->GetReverseLimitOK()))
        {
            printf("angle tilting STOP at %f\n",currentPitch);
            pitchStop();
            isPitchingUp = false;
        }
    }
    if (isPitchingDown)
    {
        pitchDown();
        if (currentPitch <= destinationPitch || !(axis->GetForwardLimitOK()))
        {
            printf("angle tilting STOP at %f\n",currentPitch);
            pitchStop();
            isPitchingDown = false;
        }
    }
    if (!isPitchingDown && !isPitchingUp)
        hasTilted = true;

    /*if(shooterJoy -> GetSmoothButton(PICKUP)) // holding down button
    {
        if (!isPickingUp) // hasn't started sequence yet
        {
            isPickingUp = true;
            pitchAngle(PICKUP_POSITION);
            clampUp();
        }
        if(!isPitchingDown) // tilt at pickup position
        {
            clampDown();
            rollerPull();
        }
    }
    else // let go of button
    {
        if(isPickingUp) // was picking up, so now should stop picking up
        {
            pitchAngle(SHOOTING_POSITION);
            isPickingUp = false;
            isPickingUpStopping = true;
        }
        if (!isPitchingUp && isPickingUpStopping) // tilt at shooting position
        {
            clampUp();
            rollerStop();
            isPickingUpStopping = false;
        }
    }*/

    if(smartFiring && smartFireTimer->HasPeriodPassed(SMARTFIRE_TIME))
    {
        punch();
        smartFiring = false;
        smartFireTimer->Stop();
    }
    
    if(wormIsPulling)
    {
        wormGear->Set(SPEED_WORM);
        if (wormDone()) //checks if loader has reached farthest position
            wormStop();
    }
}

void Shooter::updateHelper(void* instName)
{
    Shooter* shooterObj = (Shooter*)instName;
    shooterObj -> update();
}
