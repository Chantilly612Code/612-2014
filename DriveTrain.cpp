#include "DriveTrain.h"
#include <Talon.h>
double DriveTrain::PI = 3.141592653;
double DriveTrain::CIRCUMROBOT = 2 * PI * ROBOTRAD;
bool DriveTrain::isMoving = false;
bool DriveTrain::leftHasDriven = false;
bool DriveTrain::rightHasDriven = false;

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
    if (isMoving = false)
    {
        TankDrive(SPEED, SPEED);
        isMoving = true;
    }
    if  ((encode->getRDistance() >= distance) &&
         (encode->getLDistance() >= distance))
    {
        encode->EncoderL->Stop();
        encode->EncoderL->Reset();
        encode->EncoderR->Stop();
        encode->EncoderR->Reset();
        leftHasDriven = true;
        rightHasDriven = true;
        TankDrive(0.0f,0.0f);
    }
    else if (encode->getLDistance() >= distance)
    {
        encode->EncoderL->Stop();
        encode->EncoderL->Reset();
        leftHasDriven = true;
        TankDrive(0.0f, SPEED);
    }
    else if (encode->getRDistance() >= distance)
    {
        encode->EncoderR->Stop();
        encode->EncoderR->Reset();
        rightHasDriven = true;
        TankDrive(SPEED,0.0f);
    }
}
void DriveTrain::autoTurn(double degrees)
{
    double degrees2Radians = degrees * (PI/180);
    double arcLength = CIRCUMROBOT * (degrees2Radians/(2 * PI));  // checks the length of the arc in feet
    if (degrees == 0)
    {
        encode->EncoderL->Stop();
        encode->EncoderR->Stop();
        TankDrive(0.0f,0.0f);
    }
    if (degrees < 0)
    {
        encode->EncoderL->Start();
        encode->EncoderR->Start();
        TankDrive(SPEED, -SPEED); 
        if ((encode->getRDistance() >= arcLength))
        {     
            encode->EncoderL->Stop();
            encode->EncoderR->Stop();
            TankDrive(0.0f,0.0f);
        }
    }
    else if (degrees > 0)
    {    
        encode->EncoderL->Start();
        encode->EncoderR->Start();
        TankDrive(-SPEED, SPEED);
        if ((encode->getLDistance() >= arcLength))
        {      
            encode->EncoderL->Stop();
            encode->EncoderR->Stop();
            TankDrive(0.0f,0.0f);
        }
    }
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
    if (leftHasDriven == false && rightHasDriven == false)
    {
        autoDrive(NeededDist);
    }
    else 
    {
        autoTurn(NeededDegrees);
    }
}

void DriveTrain::updateHelper(void* objPtr)
{
    DriveTrain* driveObj = (DriveTrain*)objPtr;
    driveObj->update();
}


