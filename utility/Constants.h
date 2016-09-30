// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef H_BRIDGE_CONTROLLER_CONSTANTS_H
#define H_BRIDGE_CONTROLLER_CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"


namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{METHOD_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const Modular::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const Modular::HardwareInfo hardware_info;

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *

// Parameters

// Methods

// Errors
}
#endif
