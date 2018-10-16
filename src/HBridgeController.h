// ----------------------------------------------------------------------------
// HBridgeController.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef H_BRIDGE_CONTROLLER_H
#define H_BRIDGE_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "HBridgeController/Constants.h"


class HBridgeController : public ModularDeviceBase
{
public:
  HBridgeController();
  virtual void setup();
  virtual void update();
  void setChannelOn(size_t channel,
    const ConstantString * const polarity_ptr);
  void setChannelOff(size_t channel);
  void setChannelsOn(uint32_t channels,
    const ConstantString * const polarity_ptr);
  void setChannelsOff(uint32_t channels);
  void setAllChannelsOn(const ConstantString * const polarity_ptr);
  void setAllChannelsOff();
  bool channelOn(size_t channel);
  uint32_t channelsOn();

  int addPwm(uint32_t channels,
    const ConstantString * const polarity_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int startPwm(uint32_t channels,
    const ConstantString * const polarity_ptr,
    long delay,
    long period,
    long on_duration);
  int addTogglePwm(uint32_t channels,
    const ConstantString * const polarity_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int startTogglePwm(uint32_t channels,
    const ConstantString * const polarity_ptr,
    long delay,
    long period,
    long on_duration);
  void stopPwm(int pwm_index);
  void stopAllPwm();
  uint32_t arrayToChannels(ArduinoJson::JsonArray & channels_array);

  bool boardSwitchEnabled(size_t channel);
  bool boardSwitchAndPropertyEnabled(size_t channel);

  // Handlers
  virtual void startPwmHandler(int index);
  virtual void stopPwmHandler(int index);

private:
  modular_server::Pin pins_[h_bridge_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[h_bridge_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[h_bridge_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[h_bridge_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[h_bridge_controller::constants::CALLBACK_COUNT_MAX];

  EventController<h_bridge_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<h_bridge_controller::constants::PulseInfo,
    h_bridge_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool board_switch_enabled_[h_bridge_controller::constants::CHANNEL_COUNT];
  bool channels_on_[h_bridge_controller::constants::CHANNEL_COUNT];

  // Handlers
  void setChannelOnHandler();
  void setChannelOffHandler();
  void setChannelsOnHandler();
  void setChannelsOffHandler();
  void setAllChannelsOnHandler();
  void setAllChannelsOffHandler();
  void channelOnHandler();
  void channelsOnHandler();
  void addPwmHandler();
  void startPwmHandler();
  void addTogglePwmHandler();
  void startTogglePwmHandler();
  void stopPwmHandler();
  void stopAllPwmHandler();
  void boardSwitchEnabledHandler();
  void boardSwitchAndPropertyEnabledHandler();
  void setChannelsOnHandler(int index);
  void setChannelsOffHandler(int index);
  void setChannelsOnReversedHandler(int index);

};

#endif
