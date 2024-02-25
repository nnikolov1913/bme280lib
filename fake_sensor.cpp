#include <iostream>
#include "IDrukSensor.h"
#include "fake_sensor.h"

using namespace druksensor;

fakeSensor::fakeSensor(): mOpened(false)
{
    mMax = 40;
    mMin = -20;
    mTemperature = 20;
    mInc = true;
    std::cout << "fakeSensor constructor" << std::endl;
    open();
}

fakeSensor::~fakeSensor()
{
    std::cout << "fakeSensor destructor" << std::endl;
    fakeSensor::close();
}

bool fakeSensor::open()
{
    mOpened = true;
    std::cout << "fakeSensor opened " << mOpened << std::endl;
    return true;
}

void fakeSensor::close()
{
    mOpened = false;
}

bool fakeSensor::getTemperature(double &t)
{
    if (mOpened == false) {
        std::cout << "fakeSensor not initialize to get temperature" << std::endl;
        return false;
    }
    if (mInc == true) {
        mTemperature++;
        if (mTemperature > mMax) {
            mInc = false;
            mTemperature = mMax - 1;
        }
    }
    if (mInc == false) {
        mTemperature--;
        if (mTemperature < mMin) {
            mInc = true;
            mTemperature = mMin + 1;
        }
    }
    t = (double)mTemperature;
    return true;
}

