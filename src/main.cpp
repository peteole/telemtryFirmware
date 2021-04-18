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
FloatSensorValue heading("heading");
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
int counter = 0;
// Define pins
#define HC12 SerialUSB

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
  //read all sensors
  dmp.readAccel();
  dmp.readGyro();
  dmp.processRotations();
  dmp.processAcceleration();
  if (true||counter % 100 == 0)
  {
    // only send messages in 1% of the iterations
    dmp.updateReadables();
    pitch.value = dmp.pitch;
    bank.value = dmp.bank;
    heading.value = dmp.yaw * 180 / PI;
    time.value = millis();
    altitude.value = bmp.readAltitude();
    basics.send(&HC12);
  }
  //read messages and respond to them
  registry.readDataInStream(&HC12);
  while (registry.stream->available())
  {
    String message = registry.stream->readMessage();
    if (message == "calibrateGyro")
    {
      int result = dmp.mpu.calibrateGyro();
      registry.stream->addMessage("calibrated gyro, new biases: (" + String(dmp.mpu.getGyroBiasX_rads()) + "\t" + String(dmp.mpu.getGyroBiasY_rads()) + "\t" + String(dmp.mpu.getGyroBiasZ_rads()) + "), result: " + String(result));
    }
    registry.stream->addMessage("revieved: " + message);
  }
  registry.stream->send(&HC12);
  delay(10);
  counter++;
} 