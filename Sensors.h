#ifndef SENSORS_H
#define SENSORS_H

#include <AnalogChannel.h>
#include <Gyro.h>

#include "ports.h"

class main_robot;

class Sensors
{
public:
    AnalogChannel* ultrasonic;
    AnalogChannel* infraredShooter;
    AnalogChannel* infraredLoad;
    
    static const double IR_CONVERSION = 7.38976; //volts per meter IR Sensor (on shooter)
    static const double ULTRASONIC_CONVERSION = 0.0098; //volts per inch ultrasonic 

    Sensors(main_robot* robot,
            uint8_t usMod, uint32_t usChan, uint8_t isMod, uint32_t isChan,
            uint8_t ilMod, uint32_t ilChan,
            uint8_t gyMod, uint32_t gyChan);
    ~Sensors();

    float getVoltsInfShooter();//Get volts from infered shooter
    float getVoltsInfLoad();//get volts inf load
    float getVoltsUltra();//Get volts from ultra sonic
    float getInfraredShooter();
    bool getInfraredLoad();
    float getUltrasonic();
    

    Gyro* gyro612;

    float getGyroAngle();
    float getGyroRate();
    void gyroReset();
    void setGyroSens(float vpdps); //Set the Gyro Sensitivitutbgsvi sh


    static void updateHelper(void* instName);

    static const float LOAD_THRESHOLD = 10;
};

#endif
