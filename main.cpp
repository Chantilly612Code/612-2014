#include "main.h"
#include "612.h"
#include <DigitalInput.h>
#include <Relay.h>
#include <Joystick.h>
#include "ports.h"

main_robot* robot=NULL;

main_robot::main_robot()
{
    printf("hello\n");
    printf("world\n");
    robot=this;
}

main_robot::~main_robot()
{
}

void main_robot::RobotInit()
{
    printf("robot init\n");
    update = new FunctionRegistry();
    driverJoy = new SmoothJoystick(DRIVER_JOY_PORT);
    gunnerJoy = new SmoothJoystick(GUNNER_JOY_PORT);
    pnum = new Pneumatics(PNUM_DIGIN_MODULE, PNUM_DIGIN_CHANNEL, PNUM_RELAY_MODULE, PNUM_RELAY_CHANNEL);
    shift = new Shifter(SHIFT_MOD, SHIFT_FCHAN, SHIFT_RCHAN);
    shift->setHigh();
    drive = new DriveTrain(TALON_FL_MODULE, TALON_FL_CHANNEL,
                           TALON_RL_MODULE, TALON_RL_CHANNEL,
                           TALON_FR_MODULE, TALON_FR_CHANNEL,
                           TALON_RR_MODULE, TALON_RR_CHANNEL);
    shoot = new Shooter(SHOOT_JAG_MODULE,
                        SHOOT_TALON_MODULE, SHOOT_TALON_CHANNEL,
                        SHOOT_SLNOID_MODULE, SHOOT_SLNOID_FCHAN, SHOOT_SLNOID_RCHAN,
                        GUNNER_JOY_PORT,
                        SHOOT_ENCODE_MOD_A, SHOOT_ENCODE_CHAN_A, SHOOT_ENCODE_MOD_B, SHOOT_ENCODE_CHAN_B);
    printf("robot init exit\n");
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
    printf("Teleop periodic 1\n");
    update->updateFunctions();
    printf("Teleop periodic 2\n");
    float left = driverJoy->GetRawAxis(2);
    printf("Teleop periodic 3\n");
    float right = driverJoy->GetRawAxis(5);
    printf("Teleop periodic 4\n");
    drive->TankDrive(left, right);
    printf("Teleop periodic end :)\n");
}

void main_robot::AutonomousPeriodic()
{
}

void main_robot::DisabledPeriodic()
{

}
void main_robot::TestPeriodic()
{
    static int output=0;
    if(output%20==0)
    {
        printf("test periodic\n");
    }
    output++;
    pnum->checkPressure();
    pnum->updateSolenoid();
    if(gunnerJoy->GetRawButton(5))
    {
        if(shift->gear!=Shifter::low)
        {
            shift->setLow();
        }
    }
    else if(gunnerJoy->GetRawButton(6))
    {
        if(shift->gear!=Shifter::high)
        {
            shift->setHigh();
        }
    }
}

START_ROBOT_CLASS(main_robot)
