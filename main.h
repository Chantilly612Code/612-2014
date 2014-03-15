#ifndef MAIN_H
#define MAIN_H

#include <IterativeRobot.h>
#include <Joystick.h>
#include <DigitalInput.h>
#include <Relay.h>

#include "DriveTrain.h"
#include "Pneumatics.h"
#include "Shifter.h"
#include "Shooter.h"
#include "FunctionRegistry.h"
#include "Sensors.h"
#include "Netcom.h"
#include "Autonomous.h"

class main_robot: public IterativeRobot {
public:
    main_robot();
    ~main_robot();
    
    void RobotInit();
    
    void TeleopInit();
    void TeleopPeriodic();
    
    void AutonomousInit();
    void AutonomousPeriodic();
    
    void TestInit();
    void TestPeriodic();
    
    void DisabledInit(); 
    void DisabledPeriodic();
    
    // Update Registry
        FunctionRegistry* update;
    //Joysticks
    SmoothJoystick* driverJoy;
    SmoothJoystick* gunnerJoy;
    //Pneumatics
    Pneumatics* pnum;
    // Shifter
    Shifter* shift;
    // Drivetrain
    DriveTrain* drive;
    // Shooter
    Shooter* shoot;
    // Sensors
    Sensors* sensors;
    // Network Communications
    Netcom* netcom;
    // Autonomous
    Autonomous* autoBot;
};

#endif // MAIN_H
