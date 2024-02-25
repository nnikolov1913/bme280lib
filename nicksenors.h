#include <thread>
#include <atomic>
#include <functional>
#include "IDrukSensor.h"

namespace druksensor {

class NickSensors
{
    public:
        //TODO remove default constructor
        ~NickSensors();

        //Costructor, initializes the particular senzor depending on the type
        NickSensors(IDrukSensor::SensorType type);

        //If true is returned, measured temperature is in t
        bool getTemperature(double &t);

        /*
        Sets threshold value t
        thresh - direction of the threshold (above or below t) 
        callback and data for the alarm when temperature goes above/below t
        */
        void setThreshold(double t, IDrukSensor::ThresholdDir thresh, std::function<void(void *, double t)> callback, void *data);
        //TODO
        //removeThreshold

    private:
        IDrukSensor::SensorType mType;
        IDrukSensor *mSensor;
        std::thread mThread;
        std::atomic_bool mExit;
        void thresholdThread();
        std::function<void(void *, double t)> mCallback;    //Callback function to trigger the alarm
        void *mData;                                        //Callback data from the caller
        double mThreshold;                          //Threshold value to check against
        IDrukSensor::ThresholdDir mThreshDir;       //Theshold direction to get alarm above or below depending on mThreshold
        //TODO mutex condition
};

}
