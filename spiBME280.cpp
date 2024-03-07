#include <iostream>
#include "bme280_defs.h"
#include "linux_userspace.h"
#include "IDrukSensor.h"
#include "spiBME280.h"

using namespace druksensor;

spiBME280::spiBME280(): mOpened(false), mCtx(nullptr)
{
    mInterface = "/dev/spidev0.0";
}

spiBME280::~spiBME280()
{
    spiBME280::close();
}

bool spiBME280::open()
{
    if(mOpened == false) {
        int ret = bme280_main(mInterface.c_str(), 0, &mCtx);
        if (ret != BME280_OK) {
            std::cout << "BME280 SPI open error " << ret << mOpened << std::endl;
            return false;
        }
    }
    mOpened = true;
    std::cout << "BME280 SPI opened " << mOpened << std::endl;
    return true;
}

void spiBME280::close()
{
    if(mOpened == true) {
        std::cout << "BME280 SPI close" << std::endl;
        bme280_close(mCtx);
    }
    mOpened = false;
    mCtx = nullptr;
}

bool spiBME280::getTemperature(double &t)
{
    if (mOpened == false) {
        std::cout << "BME280 SPI not initialize to get temperature" << std::endl;
        return false;
    }
    struct bme280_data comp_data { 0.0, 0.0, 0.0 };
    if (bme280_read_data(mCtx, &comp_data) != BME280_OK) {
        return false;
    }
    t = comp_data.temperature;
    return true;
}

void spiBME280::setParameter(ParameterType paramtype, int param) {
    switch (paramtype) {
        case INTERFACE:
            mInterface = "/dev/spidev" + std::to_string(param) + ".0";
            std::cout << "BME280 SPI interface set to " << mInterface <<std::endl;
            break;
        case ADDRESS:
            break;
    }
}
