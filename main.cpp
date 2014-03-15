#include <DigitalInput.h>
#include <Relay.h>
#include <Joystick.h>

#include "main.h"
#include "612.h"
#include "ports.h"
#include "Netcom.h"
#include "Sensors.h"

main_robot* robot = NULL;

main_robot::main_robot() {
    robot = this;
}

main_robot::~main_robot() {
    delete driverJoy;
    delete gunnerJoy;
    delete pnum;
    delete shift;
    delete drive;
    delete shoot;
    delete update;
    delete sensors;
    delete netcom;
    delete autoBot;
}

void main_robot::RobotInit() {
    
    update = new FunctionRegistry();
    
    driverJoy = new SmoothJoystick(this, DRIVER_JOY_PORT);
    gunnerJoy = new SmoothJoystick(this, GUNNER_JOY_PORT);
    
    pnum = new Pneumatics(this, PNUM_DIGIN_MODULE, PNUM_DIGIN_CHANNEL,
            PNUM_RELAY_MODULE, PNUM_RELAY_CHANNEL);
    
    shift = new Shifter(this, SHIFT_MOD, SHIFT_FCHAN, SHIFT_RCHAN);
    shift->setHigh();
    
    drive = new DriveTrain(this, TALON_FL_MODULE, TALON_FL_CHANNEL,
            TALON_RL_MODULE, TALON_RL_CHANNEL, TALON_FR_MODULE,
            TALON_FR_CHANNEL, TALON_RR_MODULE, TALON_RR_CHANNEL);
    
    shoot = new Shooter(this, SHOOT_JAG_CAN, SHOOT_TALON_MODULE,
            SHOOT_TALON_CHANNEL, SHOOT_SLNOID_MODULE, SHOOT_SLNOID_FCHAN,
            SHOOT_SLNOID_RCHAN, WORM_JAG_CAN, PUNCH_SLNOID_MODULE,
            PUNCH_SLNOID_FCHAN, PUNCH_SLNOID_RCHAN, SHOOT_ACCEL_MODULE);
    
    sensors = new Sensors(this, USMODNUMBER, USCHANNEL, ISMODNUMBER, ISCHANNEL,
            ILMODNUMBER, ILCHANNEL, GYMOD, GYCHAN);
    
    sensors->setGyroSens(1.0f); //default sensitivity
    
    printf("Welcome to 612-2014 AERIAL ASSIST\n");
    
    netcom = new Netcom();
}

void main_robot::TeleopInit() {

}

void main_robot::TeleopPeriodic() {
    update->updateFunctions();
    float left = driverJoy->GetRawAxis(2);
    float right = driverJoy->GetRawAxis(5);
    // up is negative, down is positive
    drive->TankDrive(-left, -right);
}

void main_robot::AutonomousInit() {
    autoBot = new Autonomous(this);
    autoBot->stage = Autonomous::IDLE;
}

void main_robot::AutonomousPeriodic() {
    update -> updateFunctions();
    autoBot->stage = Autonomous::DRIVING;
    if (drive->hasDriven)
        autoBot->stage = Autonomous::TURNING;
    if (drive->hasTurned)
        autoBot->stage = Autonomous::AIMING;
    if (shoot->hasTilted)
        autoBot->stage = Autonomous::SHOOTING;

    drive->update();
}

void main_robot::TestInit() {

}

void main_robot::TestPeriodic() {
    pnum->checkPressure();
    pnum->updateSolenoid();
}

void main_robot::DisabledInit() {

}

void main_robot::DisabledPeriodic() {

}

START_ROBOT_CLASS(main_robot)
