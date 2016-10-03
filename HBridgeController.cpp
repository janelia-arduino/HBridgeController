// ----------------------------------------------------------------------------
// HBridgeController.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "HBridgeController.h"


using namespace h_bridge_controller;

HBridgeController::HBridgeController()
{
}

void HBridgeController::setup()
{
  // Parent Setup
  ModularDevice::setup();

  // Pin Setup
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    pinMode(constants::enable_pins[channel],OUTPUT);
    digitalWrite(constants::enable_pins[channel],LOW);
    pinMode(constants::dir_a_pins[channel],OUTPUT);
    pinMode(constants::dir_b_pins[channel],OUTPUT);
  }

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Device Info
  modular_server_.addFirmwareInfo(constants::firmware_info);
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Storage
  modular_server_.addFieldStorage(fields_);
  modular_server_.addParameterStorage(parameters_);
  modular_server_.addMethodStorage(methods_);

  // Fields
  modular_server::Field & polarity_reversed_field = modular_server_.createField(constants::polarity_reversed_field_name,constants::polarity_reversed_default);

  // Parameters
  modular_server::Parameter & channel_parameter = modular_server_.createParameter(constants::channel_parameter_name);
  channel_parameter.setRange(0,constants::CHANNEL_COUNT-1);

  modular_server::Parameter & polarity_parameter = modular_server_.createParameter(constants::polarity_parameter_name);
  polarity_parameter.setTypeString();
  polarity_parameter.setSubset(constants::polarity_ptr_subset);

  // Methods
  modular_server::Method & set_channel_on_method = modular_server_.createMethod(constants::set_channel_on_method_name);
  set_channel_on_method.attachCallback(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOnCallback));
  set_channel_on_method.addParameter(channel_parameter);
  set_channel_on_method.addParameter(polarity_parameter);

  modular_server::Method & set_channel_off_method = modular_server_.createMethod(constants::set_channel_off_method_name);
  set_channel_off_method.attachCallback(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOffCallback));
  set_channel_off_method.addParameter(channel_parameter);

  modular_server::Method & set_all_channels_on_method = modular_server_.createMethod(constants::set_all_channels_on_method_name);
  set_all_channels_on_method.attachCallback(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOnCallback));
  set_all_channels_on_method.addParameter(polarity_parameter);

  modular_server::Method & set_all_channels_off_method = modular_server_.createMethod(constants::set_all_channels_off_method_name);
  set_all_channels_off_method.attachCallback(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOffCallback));

}

void HBridgeController::setChannelOn(const size_t channel, const constants::Polarity polarity)
{
  bool channel_polarity_reversed;
  modular_server_.getFieldElementValue(constants::polarity_reversed_field_name,
                                       channel,
                                       channel_polarity_reversed);
  constants::Polarity polarity_corrected = polarity;
  if (channel_polarity_reversed)
  {
    polarity_corrected = ((polarity == constants::POSITIVE) ? constants::NEGATIVE : constants::POSITIVE);
  }
  if (polarity_corrected == constants::POSITIVE)
  {
    digitalWrite(constants::dir_a_pins[channel],HIGH);
    digitalWrite(constants::dir_b_pins[channel],LOW);
  }
  else
  {
    digitalWrite(constants::dir_a_pins[channel],LOW);
    digitalWrite(constants::dir_b_pins[channel],HIGH);
  }
  digitalWrite(constants::enable_pins[channel],HIGH);
}

void HBridgeController::setChannelOff(const size_t channel)
{
  digitalWrite(constants::enable_pins[channel],LOW);
}

void HBridgeController::setAllChannelsOn(const constants::Polarity polarity)
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOn(channel,polarity);
  }
}

void HBridgeController::setAllChannelsOff()
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOff(channel);
  }
}

// Callbacks must be non-blocking (avoid 'delay')
//
// modular_server_.getParameterValue must be cast to either:
// const char *
// long
// double
// bool
// ArduinoJson::JsonArray &
// ArduinoJson::JsonObject &
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.getFieldValue type must match the field default type
// modular_server_.setFieldValue type must match the field default type
// modular_server_.getFieldElementValue type must match the field array element default type
// modular_server_.setFieldElementValue type must match the field array element default type
void HBridgeController::setChannelOnCallback()
{
  long channel = modular_server_.getParameterValue(constants::channel_parameter_name);
  const char * polarity = modular_server_.getParameterValue(constants::polarity_parameter_name);
  if (polarity == constants::polarity_positive)
  {
    setChannelOn(channel,constants::POSITIVE);
  }
  else
  {
    setChannelOn(channel,constants::NEGATIVE);
  }
}

void HBridgeController::setChannelOffCallback()
{
  long channel = modular_server_.getParameterValue(constants::channel_parameter_name);
  setChannelOff(channel);
}

void HBridgeController::setAllChannelsOnCallback()
{
  const char * polarity = modular_server_.getParameterValue(constants::polarity_parameter_name);
  if (polarity == constants::polarity_positive)
  {
    setAllChannelsOn(constants::POSITIVE);
  }
  else
  {
    setAllChannelsOn(constants::NEGATIVE);
  }
}

void HBridgeController::setAllChannelsOffCallback()
{
  setAllChannelsOff();
}
