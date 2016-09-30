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


ModularDevice modular_device;

void setup()
{
  modular_device.setup();
  modular_device.startServer();
}

void loop()
{
  modular_device.update();
}
