#include "Motors.h"
#include "main.h"
#include "612.h"
#include <Relay.h>
Motors::Motors()
{
    FR = new Talon(1,7); //drivetrain
    FL = new Talon(1,1);
    RR = new Talon(1,6);
    RL = new Talon(1,2);

    drivetrain [0] = FR;
    drivetrain [1] = FL;
    drivetrain [2] = RR;
    drivetrain [3] = RL;
    
    
    grabber = new Talon(1,3);
    
    tilt = new CANJaguar(1);
    
    compressor = new Relay(1,8);
}

Motors::~Motors()
{
}

void Motors::runMotor(int motor)
{
    static bool print = false;
    static int previousMotor = -1;
    static float previousPower = 0.0;
    power = robot->driverJoy -> GetRawAxis(DRIVER_LEFT_DRIVE_AXIS);
    if ((previousMotor != motor) || previousPower != power) 
    {
        print = true;
        previousMotor = motor;
        previousPower = power;
    }
    else
    {
        print = false;
    }
    if (motor == 0 && (power > 0.1 || power < -0.1))
    {
        if (print)
            std::printf("Drivetrain\n");
        drive(print); //implementation of RobotDrive
    }
        
    else if (motor >= 1 && motor <=4)
        setTalon(motor,print);
                 
    
    else if (motor == 5)
        setTalon(grabber,print,motor);

    else if (motor == 6) //Compressor NEVER SET TO REVERSE
    {
        if (print)
            std::printf("Compressor\n");
        static bool warned = false;
        static Relay::Value compressorDirection = Relay::kReverse;
        if (robot->driverJoy->GetRawButton(BUTTON_START) && ((power > JOYSTICK_ZERO_TOLERANCE) || (power < JOYSTICK_ZERO_TOLERANCE*-1)))
        {
            compressor->Set(Relay::kForward);
            if (compressorDirection != Relay::kForward)
            {
                if (!warned)
                {
                    std::printf("RELAY OVERRIDE: WILL NOT TURN OFF AUTOMATICALLY\n");
                    warned = true;
                }
                std::printf("Compressor: kForward\n");
                compressorDirection = Relay::kForward;
            }
        }
        else if ((robot->driverJoy -> IsAxisZero(DRIVER_LEFT_DRIVE_AXIS) == false) && (robot->sense->pnumSwitch->Get() == 0))
        {
            compressor->Set(Relay::kForward);
            if (compressorDirection != Relay::kForward)
                compressorDirection = Relay::kForward;
            {
                std::printf("Compressor: kForward\n");
            }
        }
        else 
        {
            warned = false;
            compressor->Set(Relay::kOff);
            if (compressorDirection != Relay::kOff)
            {
                std::printf("Compressor: kOff\n");
                compressorDirection = Relay::kOff;
            }
        }
    }        
    
    else if (motor == 7) //tilt JAG
    {
        if (power > 0.15 || power < -0.15)
        {
            tilt -> Set(power);
            if (print)
                std::printf("7: Jag Tilt: %f\n", power);
        }
        else 
        {
            tilt->Set(0.0);
            if (print)
                std::printf("7: Jag Tilt: off\n");
        }
    }
    
    else if (motor >= 8)
    {
        std::printf("MAX\n");
        robot->selection = 70;
    }
    previousMotor = motor;
}
void Motors::drive(bool print)
{
    left = robot->driverJoy->GetRawAxis(DRIVER_LEFT_DRIVE_AXIS);
    right = robot->driverJoy->GetRawAxis(DRIVER_RIGHT_DRIVE_AXIS);
    
    FL -> Set(left);
    RL -> Set(left);
    
    FR -> Set(right);
    RR -> Set(right);
    if (print)
        std::printf("Drivetrain: %f\n", power);
}
void Motors::drive2(bool print)
{
    //TODO I want to test this first, not sure if it will work
    left = robot->driverJoy->GetRawAxis(DRIVER_LEFT_DRIVE_AXIS);
    right = robot->driverJoy->GetRawAxis(DRIVER_RIGHT_DRIVE_AXIS);

    for (int i=0;i<4;i++)
        drivetrain[i] ->Set(power);

    if (print)
        std::printf("Drivetrain Power: %f\n", power);

}
void Motors::disable()
{
    FL->Set(0.0);
    FR->Set(0.0);
    RR->Set(0.0);
    RL->Set(0.0);
    
    grabber->Set(0.0);
    
    tilt->Set(0.0);
    
    compressor->Set(Relay::kOff);
    
    robot->speed = 0.0;
}
void Motors::setTalon(Talon* t, bool print, int motor) //bool print, int motor, Talon* t = null
{
    if (print)
    {
        std::printf("%d: Talon (Roller) %u : %f\n",motor, 0, power);
    }
    if (power > 0.1 || power < -0.1)
    {
        t -> Set(power);
    }
}
void Motors::setTalon(int motor, bool print)
{
    if (print)
    {
        std::printf("%d: Talon %u : %f\n",motor, 0, power); //0 is placeholder
    }
    if (power > 0.1 || power < -0.1)
    {
        drivetrain[motor-1] -> Set(power);   
    }
}
