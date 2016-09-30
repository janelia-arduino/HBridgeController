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

#include "HBridgeController.h"


HBridgeController h_bridge_controller;

void setup()
{
  h_bridge_controller.setup();
  h_bridge_controller.startServer();
}

void loop()
{
  h_bridge_controller.update();
}
