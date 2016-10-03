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


namespace h_bridge_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=2};
enum{METHOD_COUNT_MAX=2};

enum Polarity
  {
    POSITIVE=1,
    NEGATIVE=-1
  };

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

// Units

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *
extern ConstantString polarity_reversed_field_name;

// Parameters
extern ConstantString channel_parameter_name;

extern ConstantString polarity_parameter_name;
enum{POLARITY_SUBSET_LENGTH=2};
extern ConstantString polarity_positive;
extern ConstantString polarity_negative;
extern const modular_server::SubsetMemberType polarity_ptr_subset[POLARITY_SUBSET_LENGTH];

// Methods
extern ConstantString set_channel_on_method_name;
extern ConstantString set_channel_off_method_name;

// Errors
}
}
#include "5x3.h"
#include "3x2.h"
#endif
