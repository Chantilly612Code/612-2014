#include "Autonomous.h"
#include "main.h"
#include "ports.h"
#include "612.h"

Autonomous::Autonomous(main_robot* r)
{
    robot = r;
    timer = new Timer();
    stage = IDLE;
    previousStage = stage;
}

Autonomous::~Autonomous()
{
    delete timer;
}

bool Autonomous::moveForward(double dist)
{
    if (previousStage != stage)
        robot->drive->autoDrive(dist);
    return !(robot->drive->isAuto());
}

bool Autonomous::tilt(double angle)        // needs to tilt a certain degrees, probably starting from below going up
{
    if (previousStage != stage)
        robot->shoot->pitchAngle(angle);
    if(!robot->shoot->accelWorking)
        return false;
    return robot->shoot->hasTilted;
}

bool Autonomous::wormPull()
{
    if (previousStage != stage)
    {
        robot->shoot->autoPulling=true;
        robot->shoot->wormPull();
    }
    bool wormDone = robot->shoot->wormDone();
    if(wormDone)
        robot->shoot->autoPulling=false;
    return wormDone;
}
/*
void Autonomous::vision()
{moveForward(DISTANCE)
}
*/
bool Autonomous::timePassed(float time)
{
    return (timer->HasPeriodPassed(time));
}

bool Autonomous::smartFire()
{
    if (previousStage != stage)
        robot->shoot->smartFire();
    return !robot->shoot->smartFiring;
}
/*
double Autonomous::getTime()
{
}
*/
void Autonomous::updateHighGoal()
{
    static int output=0;
    switch (stage)
    {
        case IDLE:
            printf("AUTO switch to DRIVE_AIM_WINCH\n");
            stage = DRIVE_AIM_WINCH;
            return;
        case DRIVE_AIM_WINCH:
            bool driveDone=moveForward(DISTANCE);
            bool aimDone=tilt(HIGHGOAL_AUTOANGLE);
            bool winchDone=wormPull();
            if(output%20==0)
                printf("drive: %i, aim: %i, winch: %i\n",driveDone,aimDone,winchDone);
            if(driveDone && aimDone && winchDone)
            {
                printf("AUTO switch to SMART_FIRE\n");
                stage = SMART_FIRE;
                return;
            }
            break;
        case SMART_FIRE:
            if(smartFire())
            {
                printf("AUTO done\n");
                stage = DONE;
                return;
            }
            break;
        case DONE:
            robot->drive->TankDrive(0.0,0.0);
            break;
        default:
            break;
    }
    previousStage = stage;
    output++;
}

void Autonomous::updateBasicDrive()
{
    switch (stage)
    {
        case IDLE:
            printf("AUTO switch to BASIC_DRIVE\n");
            stage = BASIC_DRIVE;
            return; // so it doesn't set the previous stage
        case BASIC_DRIVE:
            if(moveForward(DISTANCE))
            {
                printf("AUTO done\n");
                stage = DONE;
                return;
            }
            break;
        case DONE:
            robot->drive->TankDrive(0.0,0.0);
            break;
        default:
            break;
    }
    previousStage = stage;
}
