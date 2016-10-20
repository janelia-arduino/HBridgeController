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

  // Event Controller Setup
  event_controller_.setup();

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

  // Add Hardware Info
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              fields_,
                              parameters_,
                              methods_,
                              callbacks_);
  // Fields
  modular_server::Field & polarity_reversed_field = modular_server_.createField(constants::polarity_reversed_field_name,constants::polarity_reversed_default);

  modular_server::Field & channels_enabled_field = modular_server_.createField(constants::channels_enabled_field_name,constants::channels_enabled_default);
  channels_enabled_field.attachPostSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&HBridgeController::setChannelOff));

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
  delay_parameter.setUnits(constants::ms_unit);

  modular_server::Parameter & period_parameter = modular_server_.createParameter(constants::period_parameter_name);
  period_parameter.setRange(constants::period_min,constants::period_max);
  period_parameter.setUnits(constants::ms_unit);

  modular_server::Parameter & on_duration_parameter = modular_server_.createParameter(constants::on_duration_parameter_name);
  on_duration_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_duration_parameter.setUnits(constants::ms_unit);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);
  count_parameter.setUnits(constants::ms_unit);

  modular_server::Parameter & pwm_index_parameter = modular_server_.createParameter(constants::pwm_index_parameter_name);
  pwm_index_parameter.setRange(0,constants::INDEXED_PULSES_COUNT_MAX-1);

  // Methods
  modular_server::Method & set_channel_on_method = modular_server_.createMethod(constants::set_channel_on_method_name);
  set_channel_on_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOnHandler));
  set_channel_on_method.addParameter(channel_parameter);
  set_channel_on_method.addParameter(polarity_parameter);

  modular_server::Method & set_channel_off_method = modular_server_.createMethod(constants::set_channel_off_method_name);
  set_channel_off_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelOffHandler));
  set_channel_off_method.addParameter(channel_parameter);

  modular_server::Method & set_channels_on_method = modular_server_.createMethod(constants::set_channels_on_method_name);
  set_channels_on_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelsOnHandler));
  set_channels_on_method.addParameter(channels_parameter);
  set_channels_on_method.addParameter(polarity_parameter);

  modular_server::Method & set_channels_off_method = modular_server_.createMethod(constants::set_channels_off_method_name);
  set_channels_off_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setChannelsOffHandler));
  set_channels_off_method.addParameter(channels_parameter);

  modular_server::Method & set_all_channels_on_method = modular_server_.createMethod(constants::set_all_channels_on_method_name);
  set_all_channels_on_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOnHandler));
  set_all_channels_on_method.addParameter(polarity_parameter);

  modular_server::Method & set_all_channels_off_method = modular_server_.createMethod(constants::set_all_channels_off_method_name);
  set_all_channels_off_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::setAllChannelsOffHandler));

  modular_server::Method & add_pwm_method = modular_server_.createMethod(constants::add_pwm_method_name);
  add_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::addPwmHandler));
  add_pwm_method.addParameter(channels_parameter);
  add_pwm_method.addParameter(polarity_parameter);
  add_pwm_method.addParameter(delay_parameter);
  add_pwm_method.addParameter(period_parameter);
  add_pwm_method.addParameter(on_duration_parameter);
  add_pwm_method.addParameter(count_parameter);
  add_pwm_method.setReturnTypeLong();

  modular_server::Method & start_pwm_method = modular_server_.createMethod(constants::start_pwm_method_name);
  start_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::startPwmHandler));
  start_pwm_method.addParameter(channels_parameter);
  start_pwm_method.addParameter(polarity_parameter);
  start_pwm_method.addParameter(delay_parameter);
  start_pwm_method.addParameter(period_parameter);
  start_pwm_method.addParameter(on_duration_parameter);
  start_pwm_method.setReturnTypeLong();

  modular_server::Method & add_toggle_pwm_method = modular_server_.createMethod(constants::add_toggle_pwm_method_name);
  add_toggle_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::addTogglePwmHandler));
  add_toggle_pwm_method.addParameter(channels_parameter);
  add_toggle_pwm_method.addParameter(polarity_parameter);
  add_toggle_pwm_method.addParameter(delay_parameter);
  add_toggle_pwm_method.addParameter(period_parameter);
  add_toggle_pwm_method.addParameter(on_duration_parameter);
  add_toggle_pwm_method.addParameter(count_parameter);
  add_toggle_pwm_method.setReturnTypeLong();

  modular_server::Method & start_toggle_pwm_method = modular_server_.createMethod(constants::start_toggle_pwm_method_name);
  start_toggle_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::startTogglePwmHandler));
  start_toggle_pwm_method.addParameter(channels_parameter);
  start_toggle_pwm_method.addParameter(polarity_parameter);
  start_toggle_pwm_method.addParameter(delay_parameter);
  start_toggle_pwm_method.addParameter(period_parameter);
  start_toggle_pwm_method.addParameter(on_duration_parameter);
  start_toggle_pwm_method.setReturnTypeLong();

  modular_server::Method & stop_pwm_method = modular_server_.createMethod(constants::stop_pwm_method_name);
  stop_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::stopPwmHandler));
  stop_pwm_method.addParameter(pwm_index_parameter);

  modular_server::Method & stop_all_pwm_method = modular_server_.createMethod(constants::stop_all_pwm_method_name);
  stop_all_pwm_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&HBridgeController::stopAllPwmHandler));

  // Callbacks

}

void HBridgeController::setChannelOn(const size_t channel, const ConstantString * const polarity_ptr)
{
  bool channel_enabled;
  modular_server_.field(constants::channels_enabled_field_name).getElementValue(channel,
                                                                                channel_enabled);
  if (!channel_enabled)
  {
    return;
  }
  bool channel_polarity_reversed;
  modular_server_.field(constants::polarity_reversed_field_name).getElementValue(channel,
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
}

void HBridgeController::setChannelOff(const size_t channel)
{
  digitalWrite(constants::enable_pins[channel],LOW);
}

void HBridgeController::setChannelsOn(const uint32_t channels, const ConstantString * const polarity_ptr)
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

void HBridgeController::setChannelsOff(const uint32_t channels)
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

int HBridgeController::addPwm(const uint32_t channels,
                              const ConstantString * const polarity_ptr,
                              const long delay,
                              const long period,
                              const long on_duration,
                              const long count)
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

int HBridgeController::startPwm(const uint32_t channels,
                                const ConstantString * const polarity_ptr,
                                const long delay,
                                const long period,
                                const long on_duration)
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

int HBridgeController::addTogglePwm(const uint32_t channels,
                                    const ConstantString * const polarity_ptr,
                                    const long delay,
                                    const long period,
                                    const long on_duration,
                                    const long count)
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

int HBridgeController::startTogglePwm(const uint32_t channels,
                                      const ConstantString * const polarity_ptr,
                                      const long delay,
                                      const long period,
                                      const long on_duration)
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

void HBridgeController::stopPwm(const int pwm_index)
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

uint32_t HBridgeController::arrayToChannels(ArduinoJson::JsonArray & channels_array)
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  for (ArduinoJson::JsonArray::iterator channels_it=channels_array.begin();
       channels_it != channels_array.end();
       ++channels_it)
  {
    long channel = *channels_it;
    channels |= bit << channel;
  }
  return channels;
}

ConstantString * const HBridgeController:: stringToPolarityPtr(const char * string)
{
  if (string == constants::polarity_positive)
  {
    return &constants::polarity_positive;
  }
  else
  {
    return &constants::polarity_negative;
  }
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.field(field_name).getValue(value) value type must match the field default type
// modular_server_.field(field_name).setValue(value) value type must match the field default type
// modular_server_.field(field_name).getElementValue(value) value type must match the field array element default type
// modular_server_.field(field_name).setElementValue(value) value type must match the field array element default type

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
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
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
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
  setChannelsOn(channels,polarity_ptr);
}

void HBridgeController::setChannelsOffHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  setChannelsOff(channels);
}

void HBridgeController::setAllChannelsOnHandler()
{
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
  setAllChannelsOn(polarity_ptr);
}

void HBridgeController::setAllChannelsOffHandler()
{
  setAllChannelsOff();
}

void HBridgeController::addPwmHandler()
{
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
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
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
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
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
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
  ArduinoJson::JsonArray * channels_array_ptr;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array_ptr);
  const char * polarity_string;
  modular_server_.parameter(constants::polarity_parameter_name).getValue(polarity_string);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const uint32_t channels = arrayToChannels(*channels_array_ptr);
  const ConstantString * const polarity_ptr = stringToPolarityPtr(polarity_string);
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

void HBridgeController::setChannelsOnHandler(int index)
{
  uint32_t & channels = indexed_pulses_[index].channels;
  const ConstantString * const polarity_ptr = indexed_pulses_[index].polarity_ptr;
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
  const ConstantString * const polarity_ptr = indexed_pulses_[index].polarity_ptr;
  const ConstantString * polarity_reversed_ptr;
  polarity_reversed_ptr = ((polarity_ptr == &constants::polarity_positive) ? &constants::polarity_negative : &constants::polarity_positive);
  setChannelsOn(channels,polarity_reversed_ptr);
}
