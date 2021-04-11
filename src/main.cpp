#include <Arduino.h>
#include "DataSchema.h"
#include "Message.h"
#include "MessageRegistry.h"
#include "dmp/MPU9250DMP.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>

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

//initialize wire
TwoWire customWire(PB9, PB8);

//initialize dmp
MPU9250DMP dmp = MPU9250DMP(customWire);

// Initialize pressure sensor
Adafruit_BMP280 bmp(&customWire);

// Define pins
#define HC12 Serial3

void setup()
{
  //Establish data connection first
  HC12.begin(9600);
  registry.addMessage(&basics);
  registry.sendDataSchema(&HC12);

  //now initialize the sensors
  customWire.begin();
  dmp.begin();
  if (!bmp.begin(0x76))
    registry.stream->addMessage("Error initializing bmp sensor");
}

void loop()
{
  //read all sensors and write output to transfer messages
  dmp.readAccel();
  dmp.readGyro();
  dmp.updateReadables();
  pitch.value = dmp.pitch;
  bank.value = dmp.bank;
  heading.value = dmp.yaw * 180 / PI;

  altitude.value = bmp.readAltitude();

  //write messages
  basics.send(&HC12);
  registry.stream->send(&HC12);
  registry.readDataInStream(&HC12);
  while (registry.stream->available())
  {
    registry.stream->addMessage("revieved: " + registry.stream->readMessage());
  }
}