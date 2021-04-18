// file for motion procession for artificial horizon and compass
#define MPU9250_SENSOR
#ifndef MPU9250DMP_H
#define MPU9250DMP_H
#include "dmp.h"
#include "Wire.h"
#include "MPU9250.h"

class MPU9250DMP : public DMP
{
public:
    MPU9250 mpu;
    MPU9250DMP(TwoWire &medium = Wire) : mpu(medium, 0x68)
    {
    }
    void readGyro() override;
    void readAccel() override;
    // begin sensor reading
    int begin();
    void calibrateGyro();

private:
    long lastRead;
};

#endif