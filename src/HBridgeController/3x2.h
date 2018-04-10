// ----------------------------------------------------------------------------
// 3x2.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef H_BRIDGE_CONTROLLER_3X2_CONSTANTS_H
#define H_BRIDGE_CONTROLLER_3X2_CONSTANTS_H
#include "Constants.h"


#if defined(__MK20DX256__)

namespace h_bridge_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PIN_COUNT_MAX=1};

enum {CHANNEL_COUNT=2};

extern const size_t enable_pins[CHANNEL_COUNT];
extern const size_t dir_a_pins[CHANNEL_COUNT];
extern const size_t dir_b_pins[CHANNEL_COUNT];
extern const size_t sense_pins[CHANNEL_COUNT];
extern const size_t user_enable_pins[CHANNEL_COUNT];
extern const uint8_t user_enabled_polarity[CHANNEL_COUNT];

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const bool polarity_reversed_default[CHANNEL_COUNT];

extern const bool channels_enabled_default[CHANNEL_COUNT];

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif
