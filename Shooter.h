#ifndef SHOOTER_H_INCLUDED
#define SHOOTER_H_INCLUDED

#include <CANJaguar.h>
#include <Talon.h>
#include <DoubleSolenoid.h>
#include <Encoder.h>
#include "controls.h"
#include "Pneumatics.h"
#include "SmoothJoystick.h"

class Shooter
{
public:
    Shooter(uint8_t axisMod,
                 uint8_t attractMod, uint32_t attractChan,
                 uint8_t clampMod, uint32_t clampFChan, uint32_t clampRChan,
                 uint32_t sjPort,
                 uint8_t bobModA, uint32_t bobChanA, uint8_t bobModB, uint32_t bobChanB);
    ~Shooter();
    enum Clamp {down, up};
    Clamp clamp;
    void pitchUp();
    void pitchDown();
    void pitchStop();
    void pitchAngle(int32_t angle);
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
    Encoder* bobTheEncoder;
    //Blah* puncher;
    const static float SPEED_AXISPOWER;
    const static float SPEED_ATTRACTOR = 0.5f;
    const static double TIME = 0.1;
    bool isPickingUp;
    bool isPitchingUp;
    bool isPitchingDown;
    int32_t currentAng;
    int32_t originAng;
    int32_t destinationAng;

    static void buttonHelper(void* objPtr, uint32_t button);

    void update();
    static void updateHelper(void* instName);
};

#endif // SHOOTER_H_INCLUDED
