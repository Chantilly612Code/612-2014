#include "Sensors.h"
#include "ports.h"

#include "main.h"
#include "612.h"

Sensors::Sensors()
{
    pnumSwitch = new DigitalInput(PNUM_DIGIN_MODULE, PNUM_DIGIN_CHANNEL);
    left = new Encoder(LEFT_ENCODER_A_MOD,LEFT_ENCODER_A_CHAN,LEFT_ENCODER_B_MOD,LEFT_ENCODER_B_CHAN);
    right = new Encoder(RIGHT_ENCODER_A_MOD,RIGHT_ENCODER_A_CHAN,RIGHT_ENCODER_B_MOD,RIGHT_ENCODER_B_CHAN);
    ultrasonic = new AnalogChannel(ULTRASONIC_MODULE, ULTRASONIC_CHANNEL);
    infared = new AnalogChannel(IR_MODULE,IR_CHANNEL);
    ultrasonic2 = new Ultrasonic(2,1);
}
void Sensors::runSensors(int sense)
{
    static int count = 0;
    static double prevVal = -99.9;
    static int previousSense = -1;
    if (sense == 0)
    {
        if (previousSense != sense)
        {
            std::printf("!!PnumSwitch (Digital Input)!!\n");
            prevVal = -99.9;
        }
        if ((double)pnumSwitch -> Get() != prevVal)
        {
            std::printf("PnumSwitch: %f\n",(double)pnumSwitch -> Get());
        }
        prevVal = (double)pnumSwitch -> Get();
    }
    else if(sense == 1)
    {
        if (previousSense != sense)
        {
            std::printf("!!Left Encoder!!\n");
            prevVal = -99.9;
        }
        if ((double)left->Get() != prevVal)
        {
            std::printf("Left Encoder: %f\n", (double)left->Get());
        }
        prevVal = (double)left->Get();
    }
    else if (sense == 2)
    {
        if (previousSense != sense)
        {
            std::printf("!!Right Encoder!!\n");
            prevVal = -99.9;
        }
        if ((double)right->Get() != prevVal)
        {
            std::printf("Right Encoder: %f\n", (double)right->Get());
        }
        prevVal = (double)right->Get();
    }
    else if (sense == 3)
    {
        if (previousSense != sense)
        {
            std::printf("!!UltraSonic!!\n");
            prevVal = -99.9;
        }
        if (count % 50 == 0)
        {
            if ((double)ultrasonic->GetVoltage() != prevVal)
            {
                std::printf("UltraSonic Voltage: %f\nUltraSonic Distance: %f\n\n", (double)ultrasonic->GetVoltage(), (double)ultrasonic->GetVoltage()/(0.0098));
            }
        }
        prevVal = (double)ultrasonic->GetValue();
    }
    else if (sense == 4)
    {
        if (previousSense != sense)
        {
            std::printf("!!UltraSonic2!!\n");
            prevVal = -99.9;
        }
        if (count % 50 == 0)
        {
            if ((double)ultrasonic2->GetRangeInches() != prevVal)
            {
                std::printf("UltraSonic Voltage: %f\n", (double)ultrasonic2->GetRangeInches());
            }
        }
        prevVal = (double)ultrasonic2->GetRangeInches();
    }
    else if (sense == 5)
    {
        if (previousSense != sense)
        {
            std::printf("!!Infared!!\n");
            prevVal = -99.9;
        }
        if (count % 50 == 0)
        {
            double distance = (double)(infared->GetVoltage()*18.777777777777777); // about 1 cm off
            if ((double)infared->GetVoltage() != prevVal)
            {
                std::printf("Infared Voltage: %f\nInfared Value: %f\nInfared Distance (cm): %f\n\n", (double)infared->GetVoltage(), (double)infared->GetValue(), distance);
            }
        }
        prevVal = (double)infared->GetValue();
    }
    else if (sense >= 6)
    {
        std::printf("MAX\n");
        robot->selection = 50;
    }
    previousSense = sense;
    count++;
    //prevVal = sense;
}
/*
void loop() 
{
    float analogVal = analogRead(INPUTPIN);
    float volts = (analogVal/1023.0) * 5.0;
    float dist = (volts/(0.0098));
    dist = dist/12.0;
    inputs = {volts
}
*/
