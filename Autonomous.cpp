#include "Autonomous.h"
#include "main.h"
#include "ports.h"
#include "612.h"
#include "Shooter.h"
#include "ports.h"
#include <fstream>

Autonomous::Autonomous(main_robot* r): log("testLog.txt", std::ofstream::out, std::ofstream::app)
{
    robot = r;
    timer = new Timer();
    previousStage = stage = IDLE;
}

Autonomous::~Autonomous()
{
    delete timer;
}

bool Autonomous::moveForward(double dist)
{
    if (previousStage != stage)
    {
        robot->drive->autoDrive(dist);
        static int logCap;
        if(logCap % 20 == 0)
        {
            log << "function robot->drive->autoDrive has been called\n";
            log.flush();
        }
        logCap++;
    }
    return !(robot->drive->isAuto());
}

bool Autonomous::tilt(double angle)        // needs to tilt a certain degrees, probably starting from below going up
{
    if (previousStage != stage)
    {
        robot->shoot->pitchAngle(angle);
        static int logCap;
        if(logCap % 20 == 0)
        {
            log << "function robot->shoot->pitchAngle has been called\n";
            log.flush();
        }
        logCap++;
    }
    if(!robot->shoot->accelWorking)
    {
        return false;
    }
    return robot->shoot->hasTilted;
}

bool Autonomous::wormPull()
{
    if (previousStage != stage)
    {
        robot->shoot->autoPulling=true;
        robot->shoot->wormPull();
        static int logCap;
        if(logCap % 20 == 0)
        {
            log << "function robot->shoot->wormPull has been called\n";
            log.flush();
        }
        logCap++;
    }
    bool wormDone = robot->shoot->wormDone();
    if(wormDone)
    {
        robot->shoot->autoPulling=false;
    }
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
    {
        robot->shoot->smartFire();
        static int logCap;
        if(logCap % 20 == 0)
        {
            log << "function robot->shoot->smartFire has been called\n";
            log.flush();
        }
        logCap++;
    }
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
            static int logCap;
            if(logCap % 20 == 0)
            {
                log << "Auto-State is now DRIVE_AIM_WINCH\n";
                log.flush();
            }
            logCap++;
            return;
        case DRIVE_AIM_WINCH:
            bool driveDone=moveForward(DISTANCE);
            bool aimDone=tilt(HIGHGOAL_AUTOANGLE);
            bool winchDone=wormPull();
            if(output%20==0)
            {
                printf("drive: %i, aim: %i, winch: %i\n",driveDone,aimDone,winchDone);
                static int logCap;
                if(logCap % 20 == 0)
                {
                    log << "drive:" << driveDone << aimDone << winchDone << "\n";
                    log.flush();
                }
                logCap++;
            }
            if(driveDone && aimDone && winchDone)
            {
                printf("AUTO switch to SMART_FIRE\n");
                stage = SMART_FIRE;
                static int logCap;
                if(logCap % 20 == 0)
                {
                    log << "Auto-State is now SMART_FIRE";
                    log.flush();
                }
                logCap++;
                return;
            }
            break;
        case SMART_FIRE:
            if(smartFire())
            {
                printf("AUTO done\n");
                stage = DONE;
                static int logCap;
                if(logCap % 20 == 0)
                {
                    log << "Autonomous is now complete, switching to teleop\n";
                    log.flush();
                }
                logCap++;
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
            static int logCap;
            if(logCap % 20 == 0)
            {
                log << "AUTO is in BASIC_DRIVE";
                log.flush();
            }
            logCap++;
            return; // so it doesn't set the previous stage
        case BASIC_DRIVE:
            if(moveForward(DISTANCE))
//            if(tilt(Shooter::SHOOTING_POSITION))
            {
                printf("AUTO done\n");
                stage = DONE;
                static int logCap;
                if(logCap % 20 == 0)
                {
                    log << "Autonomous is done. Switch to teleop period!\n";
                    log.flush();
                }
                logCap++;
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

void Autonomous::testDataLoging()
{
    log << "Testing Data Doges";
    log.flush();
}
