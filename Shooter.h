#ifndef SHOOTER_H_INCLUDED
#define SHOOTER_H_INCLUDED

#include <CANJaguar.h>
#include <Talon.h>
#include <DoubleSolenoid.h>
#include "controls.h"
#include "Pneumatics.h"

class Shooter
{
public:
    Shooter(uint8_t axisMod,
            uint8_t attractMod, uint32_t attractChan,
            uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan);
    ~Shooter();
    enum Clamp {down, up};
    Clamp clamp;
    void pitch(bool direc);//moves it up and down
    void pull();//Wheel pulls ball
    void pullStop();
    void autoClamp();
    void clampDown();//Clamps down on ball & pulls
    void clampUp();//opens clamp
    CANJaguar* axis;
    Talon* attractor;
    DoubleSolenoid* clamper;
    Pneumatics* pneumatics;
    //Blah* puncher;
    const static float SPEED_AXISPOWER = 0.5f;
    const static float SPEED_ATTRACTOR = 0.5f;
    const static double TIME = 0.1;
};

#endif // SHOOTER_H_INCLUDED
