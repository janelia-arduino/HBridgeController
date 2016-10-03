// ----------------------------------------------------------------------------
// HBridgeController.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef H_BRIDGE_CONTROLLER_H
#define H_BRIDGE_CONTROLLER_H
#include "Flash.h"
#include <EEPROM.h>
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "JsonStream.h"
#include "Array.h"
#include "Vector.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "Functor.h"

#include "ModularServer.h"
#include "ModularDevice.h"

#include "utility/Constants.h"


class HBridgeController : public ModularDevice
{
public:
  HBridgeController();
  virtual void setup();
  void setChannelOn(size_t channel, h_bridge_controller::constants::Polarity polarity);
  void setChannelOff(size_t channel);

private:
  modular_server::Field fields_[h_bridge_controller::constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[h_bridge_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[h_bridge_controller::constants::METHOD_COUNT_MAX];

  // Callbacks
  void setChannelOnCallback();
  void setChannelOffCallback();

};

#endif
