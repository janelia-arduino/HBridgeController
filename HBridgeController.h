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

#include "IndexedContainer.h"
#include "FunctorCallbacks.h"
#include "EventController.h"

#include "ModularServer.h"
#include "ModularDevice.h"

#include "utility/Constants.h"


class HBridgeController : public ModularDevice
{
public:
  HBridgeController();
  virtual void setup();
  void setChannelOn(const size_t channel, const ConstantString * const polarity_ptr);
  void setChannelOff(const size_t channel);
  void setChannelsOn(const uint32_t channels, const ConstantString * const polarity_ptr);
  void setChannelsOff(const uint32_t channels);
  void setAllChannelsOn(const ConstantString * const polarity_ptr);
  void setAllChannelsOff();
  int addPwm(const uint32_t channels,
             const ConstantString * const polarity_ptr,
             const long delay,
             const long period,
             const long on_duration,
             const long count);
  int startPwm(const uint32_t channels,
               const ConstantString * const polarity_ptr,
               const long delay,
               const long period,
               const long on_duration);
  int addTogglePwm(const uint32_t channels,
                   const ConstantString * const polarity_ptr,
                   const long delay,
                   const long period,
                   const long on_duration,
                   const long count);
  int startTogglePwm(const uint32_t channels,
                     const ConstantString * const polarity_ptr,
                     const long delay,
                     const long period,
                     const long on_duration);
  void stopPwm(const int pwm_index);
  void stopAllPwm();
  uint32_t arrayToChannels(ArduinoJson::JsonArray & channels_array);
  ConstantString * const stringToPolarityPtr(const char * string);

  // Callbacks
  virtual void startPwmCallback(int index);
  virtual void stopPwmCallback(int index);

private:
  modular_server::Field fields_[h_bridge_controller::constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[h_bridge_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[h_bridge_controller::constants::METHOD_COUNT_MAX];

  EventController<h_bridge_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<h_bridge_controller::constants::PulseInfo,
                   h_bridge_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  // Callbacks
  void setChannelOnCallback();
  void setChannelOffCallback();
  void setChannelsOnCallback();
  void setChannelsOffCallback();
  void setAllChannelsOnCallback();
  void setAllChannelsOffCallback();
  void addPwmCallback();
  void startPwmCallback();
  void addTogglePwmCallback();
  void startTogglePwmCallback();
  void stopPwmCallback();
  void stopAllPwmCallback();
  void setChannelsOnCallback(int index);
  void setChannelsOffCallback(int index);
  void setChannelsOnReversedCallback(int index);

};

#endif
