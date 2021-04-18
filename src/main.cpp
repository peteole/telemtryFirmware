#include <Arduino.h>
#include "DataSchema.h"
#include "Message.h"
#include "MessageRegistry.h"
#include "dmp/MPU9250DMP.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SD.h>
// Initialize sensor values
FloatSensorValue pitch("pitch");
FloatSensorValue bank("bank");
FloatSensorValue speed("speed");
FloatSensorValue heading("heading");
FloatSensorValue altitude("altitude");
Int32SensorValue time("time");
SensorValue *basicValueList[] = {&pitch, &bank, &speed, &altitude, &heading, &time};
SensorValueList package(basicValueList, 6);
Message basics(&package, 5);
MessageRegistry registry;

//initialize wire
TwoWire customWire(PB9, PB8);

//initialize dmp
MPU9250DMP dmp = MPU9250DMP(customWire);

// Initialize pressure sensor
Adafruit_BMP280 bmp(&customWire);
int minIntervalMS = 200;

// Define pins
HardwareSerial HC12(PA10, PA9);

File logFile;
void setup()
{
  //Establish data connection first
  HC12.begin(9600);
  registry.addMessage(&basics);
  registry.sendDataSchema(&HC12);

  //now initialize the sensors
  customWire.begin();
  if (!dmp.begin())
    registry.stream->addMessage("Error initializing mpu9250 sensor");
  if (!bmp.begin(0x76))
    registry.stream->addMessage("Error initializing bmp sensor");
  SPI.setMISO(PB14);
  SPI.setMOSI(PB15);
  SPI.setSCLK(PB13);
  SPI.setSSEL(PB12);
  if (!SD.begin(PB12))
    registry.stream->addMessage("Error initializing SD card");
}

void loop()
{
  //read all sensors
  dmp.readAccel();
  dmp.readGyro();
  dmp.processRotations();
  dmp.processAcceleration();
  if (time.value + minIntervalMS < millis())
  {
    // only send messages in 1% of the iterations
    dmp.updateReadables();
    pitch.value = dmp.pitch;
    bank.value = dmp.bank;
    heading.value = dmp.yaw;
    time.value = millis();
    altitude.value = bmp.readAltitude();
    basics.send(&HC12);
    if (logFile)
    {
      basics.send(&logFile);
    }
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
    else if (message.startsWith("record"))
    {
      logFile = SD.open("log-" + message.substring(7) + ".txt", FILE_WRITE);
      registry.sendDataSchema(&logFile);
      if(logFile){
        registry.stream->addMessage("starting recording");
      }else{
        registry.stream->addMessage("error starting recording");
      }
    }
    else if (message == "stop")
    {
      logFile.close();
    }
    registry.stream->addMessage("recieved: " + message);
  }
  registry.stream->send(&HC12);
}