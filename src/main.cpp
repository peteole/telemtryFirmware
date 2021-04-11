#include <Arduino.h>
#include "DataSchema.h"
#include "Message.h"
#include "MessageRegistry.h"

// Initialize sensor values

FloatSensorValue pitch("pitch");
FloatSensorValue bank("bank");
FloatSensorValue speed("speed");
Int16SensorValue heading("heading");
FloatSensorValue altitude("altitude");
Int32SensorValue time("time");
SensorValue *basicValueList[] = {&pitch, &bank, &speed, &altitude, &heading, &time};
SensorValueList package(basicValueList, 6);
Message basics(&package, 17);
MessageRegistry registry;
void setup()
{
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
}