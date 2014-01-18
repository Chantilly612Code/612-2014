#include "main.h"
#include <RobotDrive.h>
#include <Talon.h>

DriveTrain::DriveTrain(uint8_t modFL,uint32_t chanFL,
                       uint8_t modRL,uint32_t chanRL,
                       uint8_t modFR,uint32_t chanFR,
                       uint8_t modRR,uint32_t chanRR)
           :RobotDrive(new Talon(modFL,chanFL),
                       new Talon(modRL,chanRL),
                       new Talon(modFR,chanFR),
                       new Talon(modRR,chanRR))
{
}

DriveTrain::~DriveTrain()
{
}

void DriveTrain::forward()
{
    //TODO  note: the following code needs ports.h or whatever
    /*
    SpeedController::Set(1,modFL);
    SpeedController::Set(1,modRL);
    SpeedController::Set(1,modFR);
    SpeedController::Set(1,modRL);
    RobotDrive::RobotDrive(SpeedController &modFL, SpeedController &modRL, SpeedController &modFR, SpeedController &modRR);
    */
}

void DriveTrain::stop()
{
    //TODO  note: the following code needs ports.h or whatever
    /*
    SpeedController::Set(0, modFL);
    SpeedController::Set(0, modRL);   
    SpeedController::Set(0, modFR);
    SpeedController::Set(0, modRL);
    RobotDrive::RobotDrive(SpeedController &modFL, SpeedController &modRL, SpeedController &modFR, SpeedController &modRR);
    */
}

void DriveTrain::autoDrive(float distance)
{
    //algorithm of autoDrive
    //parameters
    bool atDist = false;
    float enDist = encode->GetDistance();    //Set all motors to 1
    DriveTrain::forward();
    encode->Start();

    // within in a while loop,
    while (!(atDist == true));
    {
    //    - check distance (with encoders)
    enDist = encode->GetDistance();  
    //    - if were not in distance parameters, it will keep going
        if (enDist == distance);
        {
            DriveTrain::stop();
            atDist = true;
        }
    //    - else, set all motors to 0 (stopped)
    //    - end the loop
    }    
    //Below executes the motors to drive (might need a loop for distance)

}
void autoTurn(float degrees)
{
    //TODO
    /*  ~Algorithm:
    *   Check the radius of the robot (may be its own variable)
    *   Calculate the circumference
    *   Check the amount of degrees the robot turns per tick.
        - side note in terms of degrees: right is - degrees, left is + degrees
    *   Using the robot’s current position as 0 degrees, move the motors
    *    similar to autoDrive, pivot the robot in ticks(?)
    *  Then end the process.
    *Then test and make sure we are accurate enough
    */
}

void teleDrive(/*some variable*/)
{
    // TODO
}

void teleTurn(/*some variable*/)
{
    // TODO
}




