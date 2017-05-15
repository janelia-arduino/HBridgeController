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
        ]
      },
      {
        "name":"setChannelOff",
        "parameters":[
          "channel"
        ]
      },
      {
        "name":"setChannelsOn",
        "parameters":[
          "channels",
          "polarity"
        ]
      },
      {
        "name":"setChannelsOff",
        "parameters":[
          "channels"
        ]
      },
      {
        "name":"setAllChannelsOn",
        "parameters":[
          "polarity"
        ]
      },
      {
        "name":"setAllChannelsOff",
        "parameters":[]
      },
      {
        "name":"channelOn",
        "parameters":[
          "channel"
        ],
        "result_info":{
          "type":"bool"
        }
      },
      {
        "name":"channelsOn",
        "parameters":[],
        "result_info":{
          "type":"array",
          "array_element_type":"long"
        }
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
        "result_info":{
          "type":"long"
        }
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
        "result_info":{
          "type":"long"
        }
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
        "result_info":{
          "type":"long"
        }
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
        "result_info":{
          "type":"long"
        }
      },
      {
        "name":"stopPwm",
        "parameters":[
          "pwm_index"
        ]
      },
      {
        "name":"stopAllPwm",
        "parameters":[]
      },
      {
        "name":"boardSwitchEnabled",
        "parameters":[],
        "result_info":{
          "type":"array",
          "array_element_type":"bool"
        }
      },
      {
        "name":"boardSwitchAndPropertyEnabled",
        "parameters":[],
        "result_info":{
          "type":"array",
          "array_element_type":"bool"
        }
      }
    ],
    "parameters":[
      {
        "name":"channel",
        "type":"long"
      },
      {
        "name":"channels",
        "type":"array",
        "array_element_type":"long"
      },
      {
        "name":"polarity",
        "type":"string"
      },
      {
        "name":"delay",
        "type":"long",
        "units":"ms"
      },
      {
        "name":"period",
        "type":"long",
        "units":"ms"
      },
      {
        "name":"on_duration",
        "type":"long",
        "units":"ms"
      },
      {
        "name":"count",
        "type":"long",
        "units":"ms"
      },
      {
        "name":"pwm_index",
        "type":"long"
      }
    ],
    "properties":[
      {
        "name":"polarityReversed",
        "type":"array",
        "array_element_type":"bool",
        "functions":[
          "getValue",
          "setValue",
          "getDefaultValue",
          "setValueToDefault",
          "getElementValue",
          "setElementValue",
          "getDefaultElementValue",
          "setElementValueToDefault",
          "setAllElementValues",
          "getArrayLength",
          "setArrayLength"
        ],
        "parameters":[
          "value",
          "element_index",
          "element_value",
          "array_length"
        ]
      },
      {
        "name":"channelsEnabled",
        "type":"array",
        "array_element_type":"bool",
        "functions":[
          "getValue",
          "setValue",
          "getDefaultValue",
          "setValueToDefault",
          "getElementValue",
          "setElementValue",
          "getDefaultElementValue",
          "setElementValueToDefault",
          "setAllElementValues",
          "getArrayLength",
          "setArrayLength"
        ],
        "parameters":[
          "value",
          "element_index",
          "element_value",
          "array_length"
        ]
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
