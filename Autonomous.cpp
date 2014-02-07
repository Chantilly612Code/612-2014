#include "Autonomous.h"
#include "main.h"
#include "ports.h"
Autonomous::Autonomous()
{
    drive = new DriveTrain(TALON_FL_MODULE,TALON_FL_CHANNEL,
                           TALON_RL_MODULE,TALON_RL_CHANNEL,
                           TALON_FR_MODULE,TALON_FR_CHANNEL,
                           TALON_RR_MODULE,TALON_RR_CHANNEL);
    shoot = new Shooter(SHOOT_JAG_MODULE,
                        SHOOT_TALON_MODULE,  SHOOT_TALON_CHANNEL,
                        SHOOT_SLNOID_MODULE, SHOOT_SLNOID_FCHAN, SHOOT_SLNOID_RCHAN,
                        GUNNER_JOY_PORT,
                        SHOOT_ENCODE_MOD_A, SHOOT_ENCODE_CHAN_A, SHOOT_ENCODE_MOD_B, SHOOT_ENCODE_CHAN_B);
}
Autonomous::~Autonomous()
{
    delete drive;
    delete shoot;
}
void Autonomous::forward(double distance)
{
    drive->autoDrive(distance);
}
void Autonomous::turn(double degrees)
{
    drive->autoTurn(degrees);
}
void Autonomous::tilt(int degrees)        // needs to tilt a certain degrees, probably starting from below going up
{
    shoot->pitchAngle(degrees);
}
void Autonomous::releaseClamp()
{
    shoot->clampUp();
}
void Autonomous::shootBall()
{
}
/*
void Autonomous::vision()
{
}
*/
bool Autonomous::timePassed(float time)
{
    return (timer->HasPeriodPassed(time));
}
/*
double Autonomous::getTime()
{
}
*/
void Autonomous::update(State stage, double distance, double degreesTurn, int degreesTilt)
{
    switch (stage)
    {
        case DRIVING:
            forward(distance);
            break;
        case TURNING:
            turn(degreesTurn);
            break;
        case AIMING:
            tilt(degreesTilt);
            break;
        case SHOOTING:
            shootBall();
            break;
        case IDLE:
            break;
        default:
            break; 
    }
}


