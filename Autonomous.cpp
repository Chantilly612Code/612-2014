#include "Autonomous.h"
#include "main.h"
#include "ports.h"
#include "612.h"
#include "Timer.h"
#include "Shooter.h"
#include "ports.h"
#include <fstream>


std::string AUTO_TABLE_NAME="PCVision";

Autonomous::Autonomous(main_robot* r): log("testLog.txt", std::ofstream::out, std::ofstream::app), table(NetworkTable::GetTable(AUTO_TABLE_NAME))
{
    robot = r;
    timer = new Timer();
    shotTimer = new Timer();
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
    if(previousStage != stage)
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

bool Autonomous::determineHot() {
    static bool shouldWait = false;
    if (previousStage != stage) {
        shotTimer->Start();
        shouldWait = table->GetBoolean("1/WeWait",false);
    }
    static int output=0;
    if(output%5==0) {
        printf("supposed to wait: %i\n",shouldWait);
        printf("period passed: %i\n",shotTimer->HasPeriodPassed(1.0));
    }
    output++;
    if (shouldWait) {
        return shotTimer->HasPeriodPassed(1.0);
    } else {
        return true;
    }
}

bool Autonomous::wait(double time)
{
    if (previousStage != stage)
    {
        timer->Stop();
        timer->Reset();
        timer->Start();
    }
    return timer->HasPeriodPassed(time);
}

void Autonomous::updateHighGoal()
{
    static int output=0;
    switch (stage)
    {
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
        case IDLE:
            printf("AUTO switch to COARSE_AIM\n");
            stage = COARSE_AIM;
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
        case COARSE_AIM:
            if(tilt(HIGHGOAL_AUTOANGLE))
            {
                printf("AUTO switch to FINE_AIM\n");
                stage = FINE_AIM;
                static int logCap;
                if(logCap % 20 == 0)
                {
                    log << "drive:" << driveDone << aimDone << winchDone << "\n";
                    log.flush();
                }
                logCap++;
                return;
            }
            break;
        case BASIC_DRIVE:
            if(moveForward(DISTANCE))
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
        case FINE_AIM_WAIT:
            if(wait(FINE_AIM_TIME))
            {
                printf("AUTO switch to FINE_AIM\n");
                stage = FINE_AIM;
                return;
            }
            break;
        case FINE_AIM:
            if(tilt(HIGHGOAL_AUTOANGLE))
            {
                printf("AUTO switch to IS_HOT\n");
                stage = IS_HOT;
                return;
            }
            break;
        case IS_HOT:
            if (determineHot()) {
                printf("goal is hot\n");
                printf("AUTO switch to BASIC_DRIVE\n");
                stage=BASIC_DRIVE;
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
