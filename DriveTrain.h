#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <RobotDrive.h>
#include "EncodeDistance.h"
#include "ports.h"
#include "controls.h"

class DriveTrain : public RobotDrive
{
public:
    DriveTrain(uint8_t modFL,uint32_t chanFL,uint8_t modRL,uint32_t chanRL,uint8_t modFR,uint32_t chanFR,uint8_t modRR,uint32_t chanRR);
    ~DriveTrain();
    enum Dir{RIGHT, LEFT};
    void autoDrive(double distance);
    void autoTurn(double degrees);
    void teleTurn(Dir direction, double power);
    void update();
    static void updateHelper(void* objPtr);
    EncodeDistance* encode;
    static bool isMoving;
    static bool leftHasDriven;
    static bool rightHasDriven;
    static const double NeededDist = 5;
    static const double NeededDegrees = 10;
    static const float SPEED = 1.0f;
    static double PI;
    static const double ROBOTRAD = 1;   
    static double CIRCUMROBOT;
};

#endif // DRIVETRAIN_H
