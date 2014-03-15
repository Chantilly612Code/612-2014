#ifndef SMOOTHJOYSTICK_H
#define SMOOTHJOYSTICK_H

#include <Joystick.h>
#include "controls.h"
#include <vector>
#include <bitset>

class main_robot;

class SmoothJoystick: public Joystick
{
private:
    static const unsigned int NUMBUTTONS = 12;
    static const float DEADZONE = 0.1;
    static const float TRIGGER_TOLERANCE = 0.1;
public:
    SmoothJoystick(main_robot* robot, uint32_t port);
    ~SmoothJoystick();
    // Create types
    typedef void* joyfuncObjects;
    typedef void(*joyFunctions)(joyfuncObjects,uint32_t);
    typedef bool* functionBool;
    typedef void* btn_Obj;
    // Vectors to store all information
    std::vector<joyfuncObjects> Objects;
    std::vector<joyFunctions> joystickFuncs;
    std::vector<uint32_t> joyfuncButtons;
    std::vector<bool> funcBools;
    std::vector<std::bitset<3>* > buttons;

    void addJoyFunctions(joyFunctions controlFunctions, joyfuncObjects controlObjects, uint32_t btn);
    void updateJoyFunctions();
    void addButtons();
    void buttonUpdate();
    bool GetSmoothButton(int Button_number);
    trigStates GetTriggerState();
    bool IsAxisZero(uint32_t axis);
    static void updateHelper(void* instName);
};

#endif //SMOOTHJOYSTICK_H
