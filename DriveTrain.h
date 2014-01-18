#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <RobotDrive.h>
#include <EncodeDistance.h>

class DriveTrain:public RobotDrive
{
public:
    DriveTrain(uint8_t modFL,uint32_t chanFL,uint8_t modRL,uint32_t chanRL,uint8_t modFR,uint32_t chanFR,uint8_t modRR,uint32_t chanRR);
    ~DriveTrain();
    void forward();
    void stop();
    void autoDrive(float distance);        
    void autoTurn(float degrees);
    void teleDrive(/*some variable*/);
    void teleTurn(/*some variable*/);
    EncodeDistance* encode;
/*
private:
    
*/
    };

#endif // DRIVETRAIN_H
