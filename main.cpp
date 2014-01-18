#include "main.h"
#include <Joystick.h>

main_robot::main_robot()
{
}

void main_robot::RobotInit()
{
    driverJoy = new Joystick(1);
    gunnerJoy = new Joystick(2);
}
void main_robot::TeleopInit()
{
    
}
void main_robot::AutonomousInit()
{
    
}
void main_robot::TestInit()
{
    
}
void main_robot::DisabledInit()
{
    
}
void main_robot::TeleopPeriodic()
{
    float left = driverJoy->GetRawAxis(2);
    float right = driverJoy->GetRawAxis(5);
    // drive->TankDrive(left, right);
}
void main_robot::AutonomousPeriodic()
{
    
}
void main_robot::DisabledPeriodic()
{
    
}
void main_robot::TestPeriodic()
{
    
}

