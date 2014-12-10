#include <AnalogChannel.h>
#include "ports.h"
#include "Sensors.h"
#include "main.h"
#include "612.h"

// everything in inches

Sensors::Sensors(main_robot* r,
                 uint8_t usMod, uint32_t usChan, uint8_t isMod, uint32_t isChan,
                 uint8_t ilMod, uint32_t ilChan,
                 uint8_t gyMod, uint32_t gyChan,
		 int pot_slot, int pot_channel,
		 double pot_scale, double pot_offset)
{
    robot = r;

    ultrasonic = new AnalogChannel(usMod, usChan);
    infraredShooter = new AnalogChannel(isMod, isChan);
    infraredLoad = new AnalogChannel(ilMod, ilChan);
    anglePot = new AnalogPotentiometer(pot_slot, pot_channel, pot_scale, pot_offset);
    //gyro612 = new Gyro(GYMOD, GYCHAN);

//    robot -> update -> addFunctions(&updateHelper, (void*)this);
}

Sensors::~Sensors()
{
    delete ultrasonic;
    delete infraredShooter;
    delete infraredLoad;
}

//all values are in inches

float Sensors::getVoltsInfShooter()
{
    return infraredShooter->GetVoltage();
}

float Sensors::getVoltsInfLoad()
{
    return infraredLoad->GetVoltage();
}

float Sensors::getVoltsUltra()
{
    return ((ultrasonic->GetVoltage()));
}

float Sensors::getInfraredShooter()
{
    // 18.77cm/V = 7.38976in/V
    return (getVoltsInfShooter() * VPMINF );
}

bool Sensors::getInfraredLoad()
{
    float loadDistance = getVoltsInfLoad() * VPMINF ;
    if(loadDistance < LOAD_THRESHOLD)
        return true;
    else
        return false;
}

float Sensors::getUltrasonic()
{
    // 9.8mV/in = 0.0098V/in
    return ((getVoltsUltra() / VPIULTRA) + ULTRA_YINT);
}


/*void Sensors::updateHelper(void* instName)
{
    Sensors* sensorsObj = (Sensors*)instName;
    sensorsObj -> robot -> netcom -> primeLocation(sensorsObj -> getUltrasonic());
}*/

float Sensors::getGyroAngle()
{
    float gyAngle = 4.20; //gyro612->GetAngle();
    return gyAngle;
}

double Sensors::getGyroRate()
{
    double gyRate = 4.20; //gyro612->GetRate();
    return gyRate;
}

void Sensors::gyroReset()
{
    //gyro612->Reset();
}

void Sensors::setGyroSens(float vpdps /*Volts Per Degree Per Second*/)
{
    //gyro612->SetSensitivity(vpdps);
}

double Sensors::grabPot()
{
    return anglePot->Get();
}

double Sensors::findPotAngle() //Just prints Pot Value for now
{
    double tempWeed = anglePot->Get();
    printf("Current weed strength: %f \n", tempWeed);
    return tempWeed;
}
