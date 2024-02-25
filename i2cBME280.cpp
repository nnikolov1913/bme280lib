#include <iostream>
#include "bme280_defs.h"
#include "linux_userspace.h"
#include "IDrukSensor.h"
#include "i2cBME280.h"

using namespace druksensor;

i2cBME280::i2cBME280(): mOpened(false)
{
    std::cout << "BME280 I2C constructor" << std::endl;
}

i2cBME280::~i2cBME280()
{
    std::cout << "BME280 I2C destructor" << std::endl;
    i2cBME280::close();
}

bool i2cBME280::open()
{
    if(mOpened == false) {
        int ret = bme280_main("/dev/i2c-1");
        if (ret != BME280_OK) {
            std::cout << "BME280 I2C open error " << ret << mOpened << std::endl;
            return false;
        }
    }
    mOpened = true;
    std::cout << "BME280 I2C opened " << mOpened << std::endl;
    return true;
}

void i2cBME280::close()
{
    if(mOpened == true) {
        std::cout << "BME280 I2C close" << std::endl;
        bme280_close();
    }
    mOpened = false;
}

bool i2cBME280::getTemperature(double &t)
{
    if (mOpened == false) {
        std::cout << "BME280 I2C not initialize to get temperature" << std::endl;
        return false;
    }
    struct bme280_data comp_data { 0.0, 0.0, 0.0 };
    if (bme280_read_data(&comp_data) != BME280_OK) {
        return false;
    }
    t = comp_data.temperature;
    return true;
}

