#ifndef MPU6050DMP_H
#define MPU6050DMP_H
#define gyroFactor 0.00013316211  // pi/180*250/32767
#define accelFactor 0.00059877315 // 9.81*2/32767

#include "dmp.h"
#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"

class MPU6050DMP : public DMP
{
public:
	Adafruit_MPU6050 accelgyro;
	sensors_event_t accelEvent, gyro, temp;
	void readGyro() override;
	void readAccel() override;
	// begin sensor reading
	bool begin();
	void calibrateGyro();
	MPU6050DMP(TwoWire &customWire);
	TwoWire &customWire;

private:
	long lastRead;
	FloatVector rotSpeed;
};

MPU6050DMP::MPU6050DMP(TwoWire &customWire) : customWire(customWire) {}

bool MPU6050DMP::begin()
{
	lastRead = micros();
	return accelgyro.begin(0x68, &customWire);
	//readAccel();
	//down={0,accel.x,accel.y,accel.z};
}
void MPU6050DMP::readGyro()
{
	accelgyro.getGyroSensor()->getEvent(&gyro);
	long newTime = millis();
	float dt = ((float)(newTime - lastRead)) / 1000.0;
	lastRead = newTime;
	rot.x += dt * gyro.gyro.x;
	rot.y += dt * gyro.gyro.y;
	rot.z += dt * gyro.gyro.z;
	// Serial.println("rot" + toString(rot));
}
void MPU6050DMP::calibrateGyro()
{
	// todo: implement
	return;
}
void MPU6050DMP::readAccel()
{
	accelgyro.getAccelerometerSensor()->getEvent(&accelEvent);
	accel.x = accelEvent.acceleration.x;
	accel.y = accelEvent.acceleration.y;
	accel.z = accelEvent.acceleration.z;
}
#endif