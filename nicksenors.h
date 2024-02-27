#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <memory>
#include <list>
#include "IDrukSensor.h"

namespace druksensor {

class SensorAlarm {
public:
    SensorAlarm(double t, IDrukSensor::ThresholdDir dir) : mTemp(t), mThreshDir(dir)
    {
    }

    double mTemp;
    IDrukSensor::ThresholdDir mThreshDir;

    virtual void onAlarm(double t) = 0;
};

class NickSensors
{
    public:
        NickSensors() = delete;

        ~NickSensors();

        //Costructor, initializes the particular senzor depending on the type
        NickSensors(IDrukSensor::SensorType type);

        //If true is returned, measured temperature is in t
        bool getTemperature(double &t);

        //Opens the sensor for getting the temperature. Must be called before getting the temperature
        bool openSensor();

        //Closes the sensor. Optional.
        void closeSensor();

        /*
         * Sets threshold value t
         * thresh - direction of the threshold (above or below t) 
         * alarm - the alarm triggered when temperature goes above/below t
        */
        void setThreshold(SensorAlarm *alarm);

        //removes the threshold alarm set by the setThreshold
        void removeThreshold(SensorAlarm *alarm);

        //sets parameter to the sensor
        void setParameter(IDrukSensor::ParameterType paramtype, int param);

    private:
        IDrukSensor::SensorType mType;
        std::shared_ptr<IDrukSensor> mSensor;
        std::thread mThread;
        std::atomic_bool mExit;
        void thresholdThread();
        std::list<SensorAlarm *> mListAlarms;
        std::mutex mMutex;
        std::mutex mTempMutex;
        std::mutex mListMutex;
        void exitThread();
};

}
