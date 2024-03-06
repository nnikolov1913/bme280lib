#include <iostream>
#include "bme280_defs.h"
#include "linux_userspace.h"
#include "IDrukSensor.h"
#include "i2cBME280.h"

using namespace druksensor;

i2cBME280::i2cBME280(): mOpened(false), mCtx(nullptr)
{
    mAddress = BME280_I2C_ADDR_PRIM;
    mInterface = "/dev/i2c-1";
}

i2cBME280::~i2cBME280()
{
    i2cBME280::close();
}

bool i2cBME280::open()
{
    if(mOpened == false) {
        int ret = bme280_main(mInterface.c_str(), mAddress, &mCtx);
        if (ret != BME280_OK) {
            std::cout << "BME280 I2C open error " << ret << mOpened << std::endl;
            return false;
        }
    }
    mOpened = true;
    std::cout << "BME280 I2C opened " << mOpened << " address " << mAddress << std::endl;
    return true;
}

void i2cBME280::close()
{
    if(mOpened == true) {
        std::cout << "BME280 I2C close" << std::endl;
        bme280_close(mCtx);
    }
    mOpened = false;
    mCtx = nullptr;
}

bool i2cBME280::getTemperature(double &t)
{
    if (mOpened == false) {
        std::cout << "BME280 I2C not initialize to get temperature" << std::endl;
        return false;
    }
    struct bme280_data comp_data { 0.0, 0.0, 0.0 };
    if (bme280_read_data(mCtx, &comp_data) != BME280_OK) {
        return false;
    }
    t = comp_data.temperature;
    return true;
}

void i2cBME280::setParameter(ParameterType paramtype, int param) {
    switch (paramtype) {
        case INTERFACE:
            mInterface = "/dev/i2c-" + std::to_string(param);
            std::cout << "BME280 I2C interface set to " << mInterface <<std::endl;
            break;
        case ADDRESS:
            mAddress = param;
            break;
    }
}
