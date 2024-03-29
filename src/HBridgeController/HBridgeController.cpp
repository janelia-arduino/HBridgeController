// ----------------------------------------------------------------------------
// HBridgeController.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../HBridgeController.h"


using namespace h_bridge_controller;

HBridgeController::HBridgeController()
{
}

void HBridgeController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Reset Watchdog
  resetWatchdog();

  // Event Controller Setup
  event_controller_.setup();

  // Pin Setup
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    pinMode(constants::enable_pins[channel],OUTPUT);
    digitalWrite(constants::enable_pins[channel],LOW);
    pinMode(constants::dir_a_pins[channel],OUTPUT);
    pinMode(constants::dir_b_pins[channel],OUTPUT);
    pinMode(constants::user_enable_pins[channel],INPUT_PULLUP);
  }

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);
  // Properties
  modular_server_.createProperty(constants::polarity_reversed_property_name,constants::polarity_reversed_default);

  modular_server::Property & channels_enabled_property = modular_server_.createProperty(constants::channels_enabled_property_name,constants::channels_enabled_default);
  channels_enabled_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&HBridgeController::setChannelOff));

  // Parameters
  modular_server::Parameter & channel_parameter = modular_server_.createParameter(constants::channel_parameter_name);
  channel_parameter.setRange(0,constants::CHANNEL_COUNT-1);

  modular_server::Parameter & channels_parameter = modular_server_.createParameter(constants::channels_parameter_name);
  channels_parameter.setRange(0,constants::CHANNEL_COUNT-1);
  channels_parameter.setArrayLengthRange(1,constants::CHANNEL_COUNT);

  modular_server::Parameter & polarity_parameter = modular_server_.createParameter(constants::polarity_parameter_name);
  polarity_parameter.setTypeString();
  polarity_parameter.setSubset(constants::polarity_ptr_subset);

  modular_server::Parameter & delay_parameter = modular_server_.createParameter(constants::delay_parameter_name);
  delay_parameter.setRange(constants::delay_min,constants::delay_max);
  delay_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & period_parameter = modular_server_.createParameter(constants::period_parameter_name);
  period_parameter.setRange(constants::period_min,constants::period_max);
  period_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & on_duration_parameter = modular_server_.createParameter(constants::on_duration_parameter_name);
  on_duration_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_duration_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  count_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & pwm_index_parameter = modular_server_.createParameter(constants::pwm_index_parameter_name);
  pwm_index_parameter.setRange(0,constants::INDEXED_PULSES_COUNT_MAX-1);

  // Functions
  modular_server::Function & set_channel_on_function = modular_server_.createFunction(constants::set_channel_on_function_name);
  set_channel_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOnHandler));
  set_channel_on_function.addParameter(channel_parameter);
  set_channel_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_channel_off_function = modular_server_.createFunction(constants::set_channel_off_function_name);
  set_channel_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOffHandler));
  set_channel_off_function.addParameter(channel_parameter);

  modular_server::Function & set_channels_on_function = modular_server_.createFunction(constants::set_channels_on_function_name);
  set_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelsOnHandler));
  set_channels_on_function.addParameter(channels_parameter);
  set_channels_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_channels_off_function = modular_server_.createFunction(constants::set_channels_off_function_name);
  set_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelsOffHandler));
  set_channels_off_function.addParameter(channels_parameter);

  modular_server::Function & set_all_channels_on_function = modular_server_.createFunction(constants::set_all_channels_on_function_name);
  set_all_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOnHandler));
  set_all_channels_on_function.addParameter(polarity_parameter);

  modular_server::Function & set_all_channels_off_function = modular_server_.createFunction(constants::set_all_channels_off_function_name);
  set_all_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOffHandler));

  modular_server::Function & channel_on_function = modular_server_.createFunction(constants::channel_on_function_name);
  channel_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::channelOnHandler));
  channel_on_function.addParameter(channel_parameter);
  channel_on_function.setResultTypeBool();

  modular_server::Function & channels_on_function = modular_server_.createFunction(constants::channels_on_function_name);
  channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::channelsOnHandler));
  channels_on_function.setResultTypeArray();
  channels_on_function.setResultTypeLong();

  modular_server::Function & add_pwm_function = modular_server_.createFunction(constants::add_pwm_function_name);
  add_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::addPwmHandler));
  add_pwm_function.addParameter(channels_parameter);
  add_pwm_function.addParameter(polarity_parameter);
  add_pwm_function.addParameter(delay_parameter);
  add_pwm_function.addParameter(period_parameter);
  add_pwm_function.addParameter(on_duration_parameter);
  add_pwm_function.addParameter(count_parameter);
  add_pwm_function.setResultTypeLong();

  modular_server::Function & start_pwm_function = modular_server_.createFunction(constants::start_pwm_function_name);
  start_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::startPwmHandler));
  start_pwm_function.addParameter(channels_parameter);
  start_pwm_function.addParameter(polarity_parameter);
  start_pwm_function.addParameter(delay_parameter);
  start_pwm_function.addParameter(period_parameter);
  start_pwm_function.addParameter(on_duration_parameter);
  start_pwm_function.setResultTypeLong();

  modular_server::Function & add_toggle_pwm_function = modular_server_.createFunction(constants::add_toggle_pwm_function_name);
  add_toggle_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::addTogglePwmHandler));
  add_toggle_pwm_function.addParameter(channels_parameter);
  add_toggle_pwm_function.addParameter(polarity_parameter);
  add_toggle_pwm_function.addParameter(delay_parameter);
  add_toggle_pwm_function.addParameter(period_parameter);
  add_toggle_pwm_function.addParameter(on_duration_parameter);
  add_toggle_pwm_function.addParameter(count_parameter);
  add_toggle_pwm_function.setResultTypeLong();

  modular_server::Function & start_toggle_pwm_function = modular_server_.createFunction(constants::start_toggle_pwm_function_name);
  start_toggle_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::startTogglePwmHandler));
  start_toggle_pwm_function.addParameter(channels_parameter);
  start_toggle_pwm_function.addParameter(polarity_parameter);
  start_toggle_pwm_function.addParameter(delay_parameter);
  start_toggle_pwm_function.addParameter(period_parameter);
  start_toggle_pwm_function.addParameter(on_duration_parameter);
  start_toggle_pwm_function.setResultTypeLong();

  modular_server::Function & stop_pwm_function = modular_server_.createFunction(constants::stop_pwm_function_name);
  stop_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::stopPwmHandler));
  stop_pwm_function.addParameter(pwm_index_parameter);

  modular_server::Function & stop_all_pwm_function = modular_server_.createFunction(constants::stop_all_pwm_function_name);
  stop_all_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::stopAllPwmHandler));

  modular_server::Function & board_switch_enabled_function = modular_server_.createFunction(constants::board_switch_enabled_function_name);
  board_switch_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::boardSwitchEnabledHandler));
  board_switch_enabled_function.setResultTypeArray();
  board_switch_enabled_function.setResultTypeBool();

  modular_server::Function & board_switch_and_property_enabled_function = modular_server_.createFunction(constants::board_switch_and_property_enabled_function_name);
  board_switch_and_property_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::boardSwitchAndPropertyEnabledHandler));
  board_switch_and_property_enabled_function.setResultTypeArray();
  board_switch_and_property_enabled_function.setResultTypeBool();

  // Callbacks

}

void HBridgeController::update()
{
  // Parent Update
  ModularDeviceBase::update();

  bool enabled;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    enabled = (digitalRead(constants::user_enable_pins[channel]) ==
      constants::user_enabled_polarity[channel]);
    board_switch_enabled_[channel] = enabled;
    if (!enabled && channelOn(channel))
    {
      setChannelOff(channel);
    }
  }
}

void HBridgeController::setChannelOn(size_t channel,
  const ConstantString * const polarity_ptr)
{
  if (!boardSwitchAndPropertyEnabled(channel))
  {
    return;
  }
  bool channel_polarity_reversed;
  modular_server_.property(constants::polarity_reversed_property_name).getElementValue(channel,
    channel_polarity_reversed);
  const ConstantString * polarity_corrected_ptr = polarity_ptr;
  if (channel_polarity_reversed)
  {
    polarity_corrected_ptr = ((polarity_ptr == &constants::polarity_positive) ? &constants::polarity_negative : &constants::polarity_positive);
  }
  if (polarity_corrected_ptr == &constants::polarity_positive)
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
  channels_on_[channel] = true;
}

void HBridgeController::setChannelOff(size_t channel)
{
  digitalWrite(constants::enable_pins[channel],LOW);
  channels_on_[channel] = false;
}

void HBridgeController::setChannelsOn(uint32_t channels,
  const ConstantString * const polarity_ptr)
{
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channels & (bit << channel))
    {
      setChannelOn(channel,polarity_ptr);
    }
  }
}

void HBridgeController::setChannelsOff(uint32_t channels)
{
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channels & (bit << channel))
    {
      setChannelOff(channel);
    }
  }
}

void HBridgeController::setAllChannelsOn(const ConstantString * const polarity_ptr)
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOn(channel,polarity_ptr);
  }
}

void HBridgeController::setAllChannelsOff()
{
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    setChannelOff(channel);
  }
}

bool HBridgeController::channelOn(size_t channel)
{
  return channels_on_[channel];
}

uint32_t HBridgeController::channelsOn()
{
  uint32_t channels_on = 0;
  uint32_t bit = 1;
  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (channelOn(channel))
    {
      channels_on |= bit << channel;
    }
  }
  return channels_on;
}

int HBridgeController::addPwm(uint32_t channels,
  const ConstantString * const polarity_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  h_bridge_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = polarity_ptr;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOffHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int HBridgeController::startPwm(uint32_t channels,
  const ConstantString * const polarity_ptr,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  h_bridge_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = polarity_ptr;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOffHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int HBridgeController::addTogglePwm(uint32_t channels,
  const ConstantString * const polarity_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  h_bridge_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = polarity_ptr;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnReversedHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int HBridgeController::startTogglePwm(uint32_t channels,
  const ConstantString * const polarity_ptr,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  h_bridge_controller::constants::PulseInfo pulse_info;
  pulse_info.channels = channels;
  pulse_info.polarity_ptr = polarity_ptr;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnHandler),
    makeFunctor((Functor1<int> *)0,*this,&HBridgeController::setChannelsOnReversedHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&HBridgeController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

void HBridgeController::stopPwm(int pwm_index)
{
  if (pwm_index < 0)
  {
    return;
  }
  if (indexed_pulses_.indexHasValue(pwm_index))
  {
    constants::PulseInfo pulse_info = indexed_pulses_[pwm_index];
    event_controller_.remove(pulse_info.event_id_pair);
  }
}

void HBridgeController::stopAllPwm()
{
  for (size_t i=0; i<constants::INDEXED_PULSES_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
}

uint32_t HBridgeController::arrayToChannels(ArduinoJson::JsonArray channels_array)
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  for (long channel : channels_array)
  {
    channels |= bit << channel;
  }
  return channels;
}

bool HBridgeController::boardSwitchEnabled(size_t channel)
{
  return board_switch_enabled_[channel];
}

bool HBridgeController::boardSwitchAndPropertyEnabled(size_t channel)
{
  bool channel_enabled;
  modular_server_.property(constants::channels_enabled_property_name).getElementValue(channel,
    channel_enabled);
  return (channel_enabled && boardSwitchEnabled(channel));
}

void HBridgeController::startPwmHandler(int index)
{
}

void HBridgeController::stopPwmHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  setChannelsOff(channels);
  indexed_pulses_.remove(index);
}

void HBridgeController::setChannelOnHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  setChannelOn(channel,polarity_ptr);
}

void HBridgeController::setChannelOffHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOff(channel);
}

void HBridgeController::setChannelsOnHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  const uint32_t channels = arrayToChannels(channels_array);
  setChannelsOn(channels,polarity_ptr);
}

void HBridgeController::setChannelsOffHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const uint32_t channels = arrayToChannels(channels_array);
  setChannelsOff(channels);
}

void HBridgeController::setAllChannelsOnHandler()
{
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  setAllChannelsOn(polarity_ptr);
}

void HBridgeController::setAllChannelsOffHandler()
{
  setAllChannelsOff();
}

void HBridgeController::channelOnHandler()
{
  int channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  modular_server_.response().returnResult(channelOn(channel));
}

void HBridgeController::channelsOnHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(channelOn(channel));
  }

  modular_server_.response().endArray();

}

void HBridgeController::addPwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(channels_array);
  int index = addPwm(channels,polarity_ptr,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void HBridgeController::startPwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(channels_array);
  int index = startPwm(channels,polarity_ptr,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void HBridgeController::addTogglePwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(channels_array);
  int index = addTogglePwm(channels,polarity_ptr,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void HBridgeController::startTogglePwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const ConstantString * polarity_ptr;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(channels_array);
  int index = startTogglePwm(channels,polarity_ptr,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void HBridgeController::stopPwmHandler()
{
  int pwm_index;
  modular_server_.parameter(constants::pwm_index_parameter_name).getValue(pwm_index);
  stopPwm(pwm_index);
}

void HBridgeController::stopAllPwmHandler()
{
  stopAllPwm();
}

void HBridgeController::boardSwitchEnabledHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(boardSwitchEnabled(channel));
  }

  modular_server_.response().endArray();

}

void HBridgeController::boardSwitchAndPropertyEnabledHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (int channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    modular_server_.response().write(boardSwitchAndPropertyEnabled(channel));
  }

  modular_server_.response().endArray();

}

void HBridgeController::setChannelsOnHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  const ConstantString * polarity_ptr = indexed_pulses_[index].polarity_ptr;
  setChannelsOn(channels,polarity_ptr);
}

void HBridgeController::setChannelsOffHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  setChannelsOff(channels);
}

void HBridgeController::setChannelsOnReversedHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  const ConstantString * polarity_ptr = indexed_pulses_[index].polarity_ptr;
  const ConstantString * polarity_reversed_ptr;
  polarity_reversed_ptr = ((polarity_ptr == &constants::polarity_positive) ? &constants::polarity_negative : &constants::polarity_positive);
  setChannelsOn(channels,polarity_reversed_ptr);
}
