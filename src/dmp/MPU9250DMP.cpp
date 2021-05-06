#include "MPU9250DMP.h"

void MPU9250DMP::readGyro()
{
    long newTime = micros();
    float dt = ((float)(newTime - lastRead)) / 1000000.0;
    lastRead = newTime;
    mpu.update();
    rot.x += dt * mpu.getGyroX();
    rot.y += dt * mpu.getGyroY();
    rot.z += dt * mpu.getGyroZ();
}

void MPU9250DMP::readAccel()
{
    accel.x = mpu.getAccX();
    accel.y = mpu.getAccY();
    accel.z = mpu.getAccZ();
}
int MPU9250DMP::begin()
{
    lastRead = micros();
    MPU9250Setting settings;
    return mpu.setup(0x68, settings, *(this->medium));
}
void MPU9250DMP::calibrateGyro()
{
    mpu.calibrateAccelGyro();
}