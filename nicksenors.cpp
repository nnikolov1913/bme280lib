#include <iostream>
#include <chrono>
#include "i2cBME280.h"
#include "fake_sensor.h"
#include "nicksenors.h"

using namespace druksensor;

NickSensors::NickSensors(IDrukSensor::SensorType type)
{
    mExit.store(false);
    std::cout << "Library constructor, type " << type << std::endl;
    mType = type;
    if (type == IDrukSensor::BME280_INT_I2C) {
        mSensor.reset(new i2cBME280);
    }
    else if (type == IDrukSensor::FAKE_SENSOR) {
        mSensor.reset(new fakeSensor);
    }
    else {
        std::cout << "Uknown type " << type << std::endl;
        mType = IDrukSensor::NO_SENSOR;
    }
    //TODO what to do in case of error(no mSensor) ?
}

NickSensors::~NickSensors()
{
    std::cout << "Library destructor" << std::endl;

    exitThread();

    mSensor.reset();
}

bool NickSensors::openSensor()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSensor.get()) {
        return mSensor->open();
    }
    return false;
}

void NickSensors::closeSensor()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mSensor.get()) {
        mSensor->close();
    }
}

bool NickSensors::getTemperature(double &t)
{
    std::lock_guard<std::mutex> lock(mTempMutex);

    if (mSensor.get()) {
        return mSensor->getTemperature(t);
    }
    return false;
}

void NickSensors::setThreshold(SensorAlarm *alarm)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mSensor.get()) {
        return;
    }
    if (mExit.load() == true) {
        std::cout << "Alarm already started " << std::endl;
        std::lock_guard<std::mutex> lock(mListMutex);
        mListAlarms.push_back(alarm);
        return;
    }
    mListAlarms.push_back(alarm);
    mExit.store(true);
    mThread = std::thread([this]() {
        double t;
        bool ret;
        std::cout << "Thread started for sensor type " << mType << std::endl;
        while (mExit.load() == true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            {
                std::lock_guard<std::mutex> lock(mTempMutex);
                ret = mSensor->getTemperature(t);
            }
            if (false == ret) {
                //TODO Notify error ???
                continue;
            }
            {
                std::lock_guard<std::mutex> lock(mListMutex);
                for (auto alarm : mListAlarms) {
                    if ((alarm->mThreshDir == IDrukSensor::ABOVE_VALUE && t > alarm->mTemp) ||
                            (alarm->mThreshDir == IDrukSensor::BELOW_VALUE && t < alarm->mTemp)) {
                        alarm->onAlarm(t);
                    }
                }
            }
        }
        std::cout << "Thread exit for sensor type " << mType << std::endl;
    });
}

void NickSensors::removeThreshold(SensorAlarm *alarm)
{
    {
        std::lock_guard<std::mutex> lock(mListMutex);
        mListAlarms.remove(alarm);
    }
    if (mListAlarms.empty()) {
        std::lock_guard<std::mutex> lock(mMutex);
        exitThread();
    }
}

void NickSensors::exitThread()
{
    if (mExit.load() == true) {
        mExit.store(false);
        mThread.join();
    }
}

void NickSensors::setParameter(IDrukSensor::ParameterType paramtype, int param)
{
    if (mSensor.get()) {
        return mSensor->setParameter(paramtype, param);
    }
}
