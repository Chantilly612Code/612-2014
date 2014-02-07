#ifndef PORTS_H
#define PORTS_H


// DriveTrain Channels
const static uint32_t TALON_FL_CHANNEL = 1;
const static uint32_t TALON_FR_CHANNEL = 6;
const static uint32_t TALON_RL_CHANNEL = 2;
const static uint32_t TALON_RR_CHANNEL = 7;
// DriveTrain Modules
const static uint8_t  TALON_FL_MODULE  = 1;
const static uint8_t  TALON_FR_MODULE  = 1;
const static uint8_t  TALON_RL_MODULE  = 1;
const static uint8_t  TALON_RR_MODULE  = 1;
// Pneumatics Channels
const static uint32_t PNUM_DIGIN_CHANNEL = 8;
const static uint32_t PNUM_RELAY_CHANNEL = 8;
// Pneumatics Modules
const static uint8_t  PNUM_DIGIN_MODULE  = 1;
const static uint8_t  PNUM_RELAY_MODULE  = 1;

// Shifter
const static uint32_t SHIFT_FCHAN = 7;
const static uint32_t SHIFT_RCHAN = 8;
const static uint8_t  SHIFT_MOD   = 1;

// Shooter
const static uint32_t SHOOT_TALON_CHANNEL = 3;
const static uint8_t  SHOOT_TALON_MODULE  = 1;
const static uint8_t  SHOOT_JAG_MODULE    = 1;
const static uint32_t SHOOT_SLNOID_FCHAN  = 5;
const static uint32_t SHOOT_SLNOID_RCHAN  = 6;
const static uint8_t  SHOOT_SLNOID_MODULE = 1;
const static uint8_t  SHOOT_ENCODE_MOD_A  = 1;
const static uint32_t SHOOT_ENCODE_CHAN_A = 2;
const static uint8_t  SHOOT_ENCODE_MOD_B  = 1;
const static uint32_t SHOOT_ENCODE_CHAN_B = 4;
// Encoder Modules
const uint8_t  ENCODER_LMODULE_A  = 1;
const uint8_t  ENCODER_RMODULE_A  = 1;
const uint8_t  ENCODER_LMODULE_B  = 1;
const uint8_t  ENCODER_RMODULE_B  = 1;
// Encoder Channels
const uint32_t ENCODER_LCHANNEL_A = 1;
const uint32_t ENCODER_RCHANNEL_A = 2;
const uint32_t ENCODER_LCHANNEL_B = 3;
const uint32_t ENCODER_RCHANNEL_B = 4;

// Joystick
const uint32_t DRIVER_JOY_PORT = 1;
const uint32_t GUNNER_JOY_PORT = 2;

#endif
