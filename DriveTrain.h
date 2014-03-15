#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <RobotDrive.h>

#include "EncodeDistance.h"
#include "ports.h"
#include "Sensors.h"

class main_robot;

class DriveTrain : public RobotDrive
{
public:
    static const double SPEED;
    static const double PI=3.14159265;
    static const double ROBOTRAD = 3.0;
    static const double CIRCUMROBOT;
    static const double ZEROTEST = 0.001;
    
    bool isMovingL;
    bool isMovingR;
    bool isTurningL;
    bool isTurningR;
    bool hasDriven;
    bool hasTurned;
    double neededDist;
    double originUltraDist;
        
    DriveTrain(main_robot* robot,
               uint8_t modFL,uint32_t chanFL,
               uint8_t modRL,uint32_t chanRL,
               uint8_t modFR,uint32_t chanFR,
               uint8_t modRR,uint32_t chanRR);
    ~DriveTrain();
    
    enum Dir{RIGHT, LEFT};
    
    void autoDrive(double distance);
    void autoTurn(double degrees);
    void teleTurn(Dir direction, double power);
    void update();
    bool isAuto();
    void stopAuto();
    
    EncodeDistance* encode;
    Sensors* sensor;
    main_robot* robot;
    
};

#endif // DRIVETRAIN_H
