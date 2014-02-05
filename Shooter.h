#ifndef SHOOTER_H_INCLUDED
#define SHOOTER_H_INCLUDED

#include <CANJaguar.h>
#include <Talon.h>
#include <DoubleSolenoid.h>
#include "controls.h"
#include "Pneumatics.h"
#include "SmoothJoystick.h"
#include "AnalogPotentiometer.h"

class Shooter
{
public:
    Shooter(uint8_t axisMod,
                 uint8_t attractMod, uint32_t attractChan,
                 uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan,
                 uint32_t sjPort);
    ~Shooter();
    enum Clamp {down, up};
    Clamp clamp;
    void pitchUp();
    void pitchDown();
    void pitchStop();
    void pitchAngle(double angle);
    void pull();//Wheel pulls ball
    void pullStop();
    void autoClamp();
    void clampDown();//Clamps down on ball & pulls
    void clampUp();//opens clamp
    CANJaguar* axis;
    Talon* attractor;
    DoubleSolenoid* clamper;
    Pneumatics* pneumatics;
    SmoothJoystick* shooterJoy;
    AnalogPotentiometer* pot;
    //Blah* puncher;
    const static float SPEED_AXISPOWER;
    const static float SPEED_ATTRACTOR = 0.5f;
    const static double TIME = 0.1;
    double currentAng;

    static void buttonHelper(void* objPtr, uint32_t button);

    void update();
    static void updateHelper(void* instName);
};

#endif // SHOOTER_H_INCLUDED
