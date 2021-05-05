#ifndef MPU6050DMP_H
#define MPU6050DMP_H
#define gyroFactor 0.00013316211  // pi/180*250/32767
#define accelFactor 0.00059877315 // 9.81*2/32767

#include "dmp.h"
#include "MPU6050.h"
#include "Wire.h"

class MPU6050DMP : public DMP
{
public:
	MPU6050 accelgyro;
	void readGyro() override;
	void readAccel() override;
	// begin sensor reading
	void begin();
	void calibrateGyro();
	MPU6050DMP(TwoWire &customWire);

private:
	long lastRead;
	FloatVector rotSpeed;
};

MPU6050DMP::MPU6050DMP(TwoWire &customWire)
{
	accelgyro=MPU6050(0x68,customWire)
}

void MPU6050DMP::begin()
{
	accelgyro.initialize();
	lastRead = micros();
	//readAccel();
	//down={0,accel.x,accel.y,accel.z};
}
void MPU6050DMP::readGyro()
{
	int16_t x;
	int16_t y;
	int16_t z;
	accelgyro.getRotation(&x, &y, &z);
	long newTime = millis();
	float dt = ((float)(newTime - lastRead)) / 1000.0;
	lastRead = newTime;
	rot.x += dt * gyroFactor * (float)x;
	rot.y += dt * gyroFactor * (float)y;
	rot.z += dt * gyroFactor * (float)z;
	rotSpeed.x = gyroFactor * (float)x;
	rotSpeed.y = gyroFactor * (float)y;
	rotSpeed.z = gyroFactor * (float)z;
	// Serial.println("rot" + toString(rot));
}
void MPU6050DMP::calibrateGyro()
{
	accelgyro.setXGyroOffset(-13);
	accelgyro.setYGyroOffset(-18);
	accelgyro.setZGyroOffset(4);
	return;
}
void MPU6050DMP::readAccel()
{
	int16_t x;
	int16_t y;
	int16_t z;
	accelgyro.getAcceleration(&x, &y, &z);
	accel.x = accelFactor * (float)x;
	accel.y = accelFactor * (float)y;
	accel.z = accelFactor * (float)z;
	// Serial.println("a" + toString(accel));
}
#endif