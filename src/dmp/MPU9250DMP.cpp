#include "MPU9250DMP.h"

void MPU9250DMP::readGyro()
{
    long newTime = micros();
    float dt = ((float)(newTime - lastRead)) / 1000000.0;
    lastRead = newTime;
    mpu.readSensor();
    rot.x += dt * mpu.getGyroX_rads();
    rot.y += dt * mpu.getGyroY_rads();
    rot.z += dt * mpu.getGyroZ_rads();
}

void MPU9250DMP::readAccel()
{
    accel.x = mpu.getAccelX_mss();
    accel.y = mpu.getAccelY_mss();
    accel.z = mpu.getAccelZ_mss();
}
int MPU9250DMP::begin()
{
    lastRead = micros();
    return mpu.begin();
}
void MPU9250DMP::calibrateGyro(){
    mpu.calibrateGyro();
}