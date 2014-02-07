#ifndef AUTONOMOUS_H   // collabedit.com/rg9ek
#define AUTONOMOUS_H
#include "DriveTrain.h"
#include "Shooter.h"
#include <Timer.h>
class Autonomous
{
public:
    Autonomous();
    ~Autonomous();
    void forward(double distance);
    void turn(double degrees);
    void tilt(int degrees);
    void releaseClamp();
    void shootBall();
    
    bool timePassed(float time);        //time measured in seconds
//  void vision();                      //probably connected to tilt
//  double getTime();                   // might not be needed at the moment
    DriveTrain* drive;
    Shooter* shoot;
    Timer* timer;
    enum State {DRIVING, TURNING, AIMING, SHOOTING, IDLE};
//    State* stage;

    void update(State stage, double distance, double degreesTurn, int degreesTilt);    
};
#endif //AUTONOMOUS_H


