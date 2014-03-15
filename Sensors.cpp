#include <AnalogChannel.h>
#include "ports.h"
#include "Sensors.h"
#include "main.h"
#include "612.h"

// everything in inches

Sensors::Sensors(main_robot* robot,
                 uint8_t usMod, uint32_t usChan, uint8_t isMod, uint32_t isChan,
                 uint8_t ilMod, uint32_t ilChan,
                 uint8_t gyMod, uint32_t gyChan)
{
    ultrasonic = new AnalogChannel(usMod, usChan);
    infraredShooter = new AnalogChannel(isMod, isChan);
    infraredLoad = new AnalogChannel(ilMod, ilChan);
    gyro612 = new Gyro(GYMOD, GYCHAN);

    robot -> update -> addFunctions(&updateHelper, (void*)this);
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
    return (getVoltsInfShooter() * IR_CONVERSION );
}

bool Sensors::getInfraredLoad()
{
    float loadDistance = getVoltsInfLoad() * IR_CONVERSION ;
    if(loadDistance < LOAD_THRESHOLD)
        return true;
    return false;
}

float Sensors::getUltrasonic()
{
    // 9.8mV/in = 0.0098V/in
    return (getVoltsUltra() / ULTRASONIC_CONVERSION);
}


void Sensors::updateHelper(void* instName)
{
    Sensors* sensorsObj = (Sensors*)instName;
    robot -> netcom -> primeLocation(sensorsObj -> getUltrasonic());
}

float Sensors::getGyroAngle()
{
    return gyro612->GetAngle();
}

float Sensors::getGyroRate()
{
    return gyro612->GetRate();
}

void Sensors::gyroReset()
{
    gyro612->Reset();
}

void Sensors::setGyroSens(float gyroSensitivity) //Volts Per Degree Per Second
{
    gyro612->SetSensitivity(gyroSensitivity);
}
