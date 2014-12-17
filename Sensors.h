#ifndef SENSORS_H
#define SENSORS_H

#include <AnalogChannel.h>
#include <AnalogPotentiometer.h>
#include <Gyro.h>
#include "ports.h"

class Sensors
{
public:
    AnalogChannel* ultrasonic;
    AnalogChannel* infraredShooter;
    AnalogChannel* infraredLoad;
    AnalogPotentiometer* anglePot;
    static const double VPMINF = 7.38976; //voltage per meter shooter
    static const double VPIULTRA = 0.0097361;
    static const double ULTRA_YINT = 0.7008307562;

    Sensors(class main_robot* r,
            uint8_t usMod, uint32_t usChan, uint8_t isMod, uint32_t isChan,
            uint8_t ilMod, uint32_t ilChan,
            uint8_t gyMod, uint32_t gyChan,
            int pot_slot, int pot_channel,
	    double pot_scale, double pot_offset);
    ~Sensors();

    float getVoltsInfShooter();//Get volts from infered shooter
    float getVoltsInfLoad();//get volts inf load
    float getVoltsUltra();//Get volts from ultra sonic
    float getInfraredShooter();
    bool getInfraredLoad();
    float getUltrasonic();

    //Gyro* gyro612;
    class main_robot* robot;

    float getGyroAngle();
    double getGyroRate();
    void gyroReset();
    void setGyroSens(float vpdps); //Set the Gyro Sensitivitutbgsvi sh
    double grabPot();
    double findPotAngle();

    //static void updateHelper(void* instName);

    static const float LOAD_THRESHOLD = 10;
};

#endif
