#include <iostream>
#include <chrono>
#include "i2cBME280.h"
#include "fake_sensor.h"
#include "nicksenors.h"

using namespace druksensor;

NickSensors::NickSensors(IDrukSensor::SensorType type)
{
    mSensor = nullptr;
    mExit.store(false);
    std::cout << "Library constructor, type " << type << std::endl;
    mType = type;
    if (type == IDrukSensor::BME280_INT_I2C) {
        mSensor = new i2cBME280;
    }
    else if (type == IDrukSensor::FAKE_SENSOR) {
        mSensor = new fakeSensor;
    }
    else {
        std::cout << "Uknown type " << type << std::endl;
        mType = IDrukSensor::NO_SENSOR;
    }
}

NickSensors::~NickSensors()
{
    std::cout << "Library destructor" << std::endl;

    exitThread();

    if (mSensor != nullptr) {
        delete mSensor;
    }
}

bool NickSensors::openSensor()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSensor == nullptr) {
        return false;
    }
    return mSensor->open();
}

void NickSensors::closeSensor()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSensor != nullptr) {
        mSensor->close();
    }
}

bool NickSensors::getTemperature(double &t)
{
    std::lock_guard<std::mutex> lock(mTempMutex);

    if (mSensor == nullptr) {
        return false;
    }
    return mSensor->getTemperature(t);
}

void NickSensors::setThreshold(double t, IDrukSensor::ThresholdDir thresh, std::function<void(void *, double t)> callback, void *data)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mSensor == nullptr) {
        return;
    }
    if (mExit.load() == true) {
        std::cout << "Alarm already set " << std::endl;
        return;
    }
    mData = data;
    mCallback = callback;
    mThreshold = t;
    mThreshDir = thresh;
    mExit.store(true);
    mThread = std::thread(&NickSensors::thresholdThread, this);
    //wait till thread start ?
}

void NickSensors::thresholdThread()
{
    double t;
    bool ret;
    std::cout << "Thread started for sensor type " << mType << std::endl;
    while (mExit.load() == true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        {
            std::lock_guard<std::mutex> lock(mTempMutex);
            ret = mSensor->getTemperature(t);
        }
        if (true == ret) {
            if ((mThreshDir == IDrukSensor::ABOVE_VALUE && t > mThreshold) ||
                    (mThreshDir == IDrukSensor::BELOW_VALUE && t < mThreshold)) {
                mCallback(mData, t);
            }
        }
    }
    std::cout << "Thread exit for sensor type " << mType << std::endl;
}

void NickSensors::removeThreshold()
{
    std::lock_guard<std::mutex> lock(mMutex);
    exitThread();
}

void NickSensors::exitThread()
{
    if (mExit.load() == true) {
        mExit.store(false);
        mThread.join();
    }
}
