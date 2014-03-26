#ifndef AUTONOMOUS_H   // collabedit.com/rg9ek
#define AUTONOMOUS_H
#include "DriveTrain.h"
#include "Shooter.h"
#include <Timer.h>

// the auto methods return true when completed

class Autonomous
{
public:
    // Constructor and destructor
    Autonomous(main_robot* robot);
    ~Autonomous();
    // Basic commands used by many autonomous routines
    bool moveForward(double dist);
    bool turn();
    bool tilt(double);
    bool wormPull();
    bool smartFire();
    // Timing
    bool timePassed(float time);        //time measured in seconds
    Timer* timer;
    //Autonomous routines
    void updateHighGoal();
    void updateBasicDrive();

    static const double DISTANCE = 151.0;
    static const double DEGREES_TURN = 25.0;
    static const double HIGHGOAL_AUTOANGLE = 53.0;
    
private:
    main_robot* robot; //Variable to keep track of robot instance
    // Internal State Tracker
    enum State {
        DRIVE_AIM_WINCH,
        SMART_FIRE,
        BASIC_DRIVE,
        IDLE,
        DONE
    };
    State stage;
    State previousStage;
};
#endif //AUTONOMOUS_H


