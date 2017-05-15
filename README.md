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

## API

```json
{
  "id":"getApi",
  "result":{
    "firmware":["HBridgeController"],
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
    ],
    "callbacks":[]
  }
}
```

## API Verbose

```json
{
  "id":"getApiVerbose",
  "result":{
    "firmware":["HBridgeController"],
    "functions":[
      {
        "name":"setChannelOn",
        "parameters":[
          "channel",
          "polarity"
        ],
        "result_type":null
      },
      {
        "name":"setChannelOff",
        "parameters":[
          "channel"
        ],
        "result_type":null
      },
      {
        "name":"setChannelsOn",
        "parameters":[
          "channels",
          "polarity"
        ],
        "result_type":null
      },
      {
        "name":"setChannelsOff",
        "parameters":[
          "channels"
        ],
        "result_type":null
      },
      {
        "name":"setAllChannelsOn",
        "parameters":[
          "polarity"
        ],
        "result_type":null
      },
      {
        "name":"setAllChannelsOff",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"channelOn",
        "parameters":[
          "channel"
        ],
        "result_type":"bool"
      },
      {
        "name":"channelsOn",
        "parameters":[],
        "result_type":"array"
      },
      {
        "name":"addPwm",
        "parameters":[
          "channels",
          "polarity",
          "delay",
          "period",
          "on_duration",
          "count"
        ],
        "result_type":"long"
      },
      {
        "name":"startPwm",
        "parameters":[
          "channels",
          "polarity",
          "delay",
          "period",
          "on_duration"
        ],
        "result_type":"long"
      },
      {
        "name":"addTogglePwm",
        "parameters":[
          "channels",
          "polarity",
          "delay",
          "period",
          "on_duration",
          "count"
        ],
        "result_type":"long"
      },
      {
        "name":"startTogglePwm",
        "parameters":[
          "channels",
          "polarity",
          "delay",
          "period",
          "on_duration"
        ],
        "result_type":"long"
      },
      {
        "name":"stopPwm",
        "parameters":[
          "pwm_index"
        ],
        "result_type":null
      },
      {
        "name":"stopAllPwm",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"boardSwitchEnabled",
        "parameters":[],
        "result_type":"array"
      },
      {
        "name":"boardSwitchAndPropertyEnabled",
        "parameters":[],
        "result_type":"array"
      }
    ],
    "parameters":[
      {
        "name":"channel",
        "type":"long"
      },
      {
        "name":"channels",
        "type":"array"
      },
      {
        "name":"polarity",
        "type":"string"
      },
      {
        "name":"delay",
        "units":"ms",
        "type":"long"
      },
      {
        "name":"period",
        "units":"ms",
        "type":"long"
      },
      {
        "name":"on_duration",
        "units":"ms",
        "type":"long"
      },
      {
        "name":"count",
        "units":"ms",
        "type":"long"
      },
      {
        "name":"pwm_index",
        "type":"long"
      }
    ],
    "properties":[
      {
        "name":"polarityReversed",
        "type":"array"
      },
      {
        "name":"channelsEnabled",
        "type":"array"
      }
    ],
    "callbacks":[]
  }
}
```

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
