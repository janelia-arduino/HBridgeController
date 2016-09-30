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

#include "utility/Constants.h"


class HBridgeController
{
public:
  HBridgeController();
  virtual void setup();
  virtual void update();
  virtual void startServer();

private:

  // Callbacks

};

#endif
