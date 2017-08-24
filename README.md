# HBridgeController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Ancestors

[ModularServer](https://github.com/janelia-arduino/ModularServer)

[ModularDeviceBase](https://github.com/janelia-arduino/ModularDeviceBase)

## Clients

## Devices

[modular_device_base](https://github.com/janelia-modular-devices/modular_device_base.git)

[h_bridge_controller](https://github.com/janelia-modular-devices/h_bridge_controller.git)

## API NAMES

```json
{
  "id":"getApi",
  "result":{
    "firmware":["HBridgeController"],
    "verbosity":"NAMES",
    "functions":[
      "setChannelOn",
      "setChannelOff",
      "setChannelsOn",
      "setChannelsOff",
      "setAllChannelsOn",
      "setAllChannelsOff",
      "channelOn",
      "channelsOn",
      "addPwm",
      "startPwm",
      "addTogglePwm",
      "startTogglePwm",
      "stopPwm",
      "stopAllPwm",
      "boardSwitchEnabled",
      "boardSwitchAndPropertyEnabled"
    ],
    "parameters":[
      "channel",
      "channels",
      "polarity",
      "delay",
      "period",
      "on_duration",
      "count",
      "pwm_index"
    ],
    "properties":[
      "polarityReversed",
      "channelsEnabled"
    ]
  }
}
```

## API GENERAL

[API GENERAL](./api/)

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
