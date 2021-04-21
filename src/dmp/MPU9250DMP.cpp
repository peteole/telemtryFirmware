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
void MPU9250DMP::calibrateGyro()
{
    double _gxbD = 0;
    double _gybD = 0;
    double _gzbD = 0;
    int _numSamples = 100;
    float _gxb = mpu.getGyroBiasX_rads();
    float _gyb = mpu.getGyroBiasY_rads();
    float _gzb = mpu.getGyroBiasZ_rads();
    for (size_t i = 0; i < _numSamples; i++)
    {
        mpu.readSensor();
        _gxbD += (mpu.getGyroX_rads() + _gxb) / ((double)_numSamples);
        _gybD += (mpu.getGyroY_rads() + _gyb) / ((double)_numSamples);
        _gzbD += (mpu.getGyroZ_rads() + _gzb) / ((double)_numSamples);
        delay(20);
    }
    mpu.setGyroBiasX_rads((float)_gxbD);
    mpu.setGyroBiasY_rads((float)_gybD);
    mpu.setGyroBiasZ_rads((float)_gzbD);
}