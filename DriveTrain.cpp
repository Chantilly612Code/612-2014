#include "DriveTrain.h"
#include <Talon.h>
#include "612.h"
#include "main.h"

const double DriveTrain::PI = 3.141592653;
const double DriveTrain::CIRCUMROBOT = 2 * PI * ROBOTRAD;
bool DriveTrain::isMovingL = false;
bool DriveTrain::isMovingR = false;
bool DriveTrain::isTurningL = false;
bool DriveTrain::isTurningR = false;
const float DriveTrain::SPEED = 1.0f;

DriveTrain::DriveTrain(uint8_t modFL,uint32_t chanFL,
                        uint8_t modRL,uint32_t chanRL,
                        uint8_t modFR,uint32_t chanFR,
                        uint8_t modRR,uint32_t chanRR)
           :RobotDrive(new Talon(modFL,chanFL),
                        new Talon(modRL,chanRL),
                        new Talon(modFR,chanFR),
                        new Talon(modRR,chanRR))
{
    encode = new EncodeDistance(ENCODER_LMODULE_A, ENCODER_LCHANNEL_A,
                                ENCODER_LMODULE_B, ENCODER_LCHANNEL_B,
                                ENCODER_RMODULE_A, ENCODER_RCHANNEL_A,
                                ENCODER_RMODULE_B, ENCODER_RCHANNEL_B);

    robot -> update -> addFunctions(&updateHelper, (void*) this);
}

DriveTrain::~DriveTrain()
{
    delete encode;
}

void DriveTrain::autoDrive(double distance)
{
    NeededDist = distance;
    TankDrive(SPEED, SPEED);
    isMovingL = true;
    isMovingR = true;
    encode->EncoderL->Start();
    encode->EncoderR->Start();
}
void DriveTrain::autoTurn(double degrees)                         // any degrees less than zero (0) will turn right; basically the unit circle
{
    double degrees2Radians = degrees * (PI/180);
    double arcLength = CIRCUMROBOT * (degrees2Radians/(2 * PI));  // checks the length of the arc in feet
    NeededDist = arcLength;
    if (degrees > 0){
        TankDrive(-SPEED, SPEED);
        isTurningL = true;
    }
    if (degrees < 0){
        TankDrive(SPEED, -SPEED);
        isTurningR = true;
    }
    encode->EncoderL->Start();
    encode->EncoderR->Start();
}

void DriveTrain::teleTurn(Dir direction, double power)
{
    if (isAuto())
    {
        stopAuto();
    }
    if (direction == RIGHT)
    {
        TankDrive(power,-1*power);
    }
    else if (direction == LEFT)
    {
        TankDrive(-1*power,power);
    }
}

void DriveTrain::update()
{
    float speedL;
    float speedR;
    if (isMovingL || isMovingR)
    {
        speedL = SPEED;
        if (encode->getLDistance() >= NeededDist)
        {
            encode->EncoderL->Stop();
            encode->EncoderL->Reset();
            isMovingL = false;
            speedL = 0.0f;
        }
        speedR = SPEED;
        if (encode->getRDistance() >= NeededDist)
        {
            encode->EncoderR->Stop();
            encode->EncoderR->Reset();
            isMovingR = false;
            speedR = 0.0f;
        }
        TankDrive(speedL, speedR);
    }
    if (isTurningL) // NeededDist is positive
    {
        speedL = SPEED;
        if (encode->getLDistance() <= -NeededDist)
        {
            encode->EncoderL->Stop();
            encode->EncoderL->Reset();
            speedL = 0.0f;
        }
        speedR = SPEED;
        if (encode->getRDistance() >= NeededDist)
        {
            encode->EncoderR->Stop();
            encode->EncoderR->Reset();
            speedR = 0.0f;
        }
        isTurningL = false;
        TankDrive(-speedL, speedR);
    }
    else
    {
        speedL = SPEED;
        if (encode->getLDistance() >= -NeededDist)
        {
            encode->EncoderL->Stop();
            encode->EncoderL->Reset();
            speedL = 0.0f;
        }
        speedR = SPEED;
        if (encode->getRDistance() <= NeededDist)
        {
            encode->EncoderR->Stop();
            encode->EncoderR->Reset();
            speedR = 0.0f;
        }
        isTurningR = false;
        TankDrive(-speedL, speedR);
    }
}

void DriveTrain::updateHelper(void* instName)
{
    DriveTrain* driveObj = (DriveTrain*)instName;
    driveObj->update();
}

bool DriveTrain::isAuto()
{
    return ((isMovingL) || (isMovingR) || (isTurningL) || (isTurningR));
}

void DriveTrain::stopAuto()
{
    TankDrive(0.0f, 0.0f);
    encode->EncoderL->Stop();
    encode->EncoderR->Stop();
    encode->EncoderL->Reset();
    encode->EncoderR->Reset();
    isMovingL = false;
    isMovingR = false;
    isTurningL = false;
    isTurningR = false;
}
