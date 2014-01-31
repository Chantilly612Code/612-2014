#include "DriveTrain.h"
#include <Talon.h>
double DriveTrain::PI = 3.141592653;
double DriveTrain::CIRCUMROBOT = 2 * PI * ROBOTRAD;
bool DriveTrain::isMovingL = false;
bool DriveTrain::isMovingR = false;
bool DriveTrain::isTurningL = false;
bool DriveTrain::isTurningR = false;

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
void DriveTrain::autoTurn(double degrees)
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
    if (isTurningL || isTurningR)
    {
        speedL = SPEED;
        if (encode->getLDistance() >= NeededDist)
        {
            encode->EncoderL->Stop();
            encode->EncoderL->Reset();
            isTurningL = false;
            speedL = 0.0f;
        }
        speedR == SPEED;
        if (encode->getRDistance() >= -NeededDist)
        {
            encode->EncoderR->Stop();
            encode->EncoderR->Reset();
            isTurningR = false;
            speedR = 0.0f;
        }
        if (isTurningL)
        {
            TankDrive(-speedL, speedR);
        }
        if (isTurningR)
        {
            TankDrive(speedL, -speedR);
        }
    }
}

void DriveTrain::updateHelper(void* objPtr)
{
    DriveTrain* driveObj = (DriveTrain*)objPtr;
    driveObj->update();
}


