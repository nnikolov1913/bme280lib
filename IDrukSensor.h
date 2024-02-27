#ifndef __IDRUKSENSOR_H__
#define __IDRUKSENSOR_H__

/*
 * Interface to manipulate the sensors
 */

namespace druksensor {

class IDrukSensor {
    public:

        //Sensor type
        enum SensorType {
            NO_SENSOR,
            BME280_INT_I2C,         //BME280 sensor
            FAKE_SENSOR,            //Fake sensor for tests purposes
        };

        //Used to set the type of the alarm
        enum ThresholdDir {
            ABOVE_VALUE,            //Set value to get alarm above it
            BELOW_VALUE,            //Set value to get alarm below it
        };

        //Used to set parameter to the sensor
        enum ParameterType {
            INTERFACE,
            ADDRESS,
        };

        virtual ~IDrukSensor() = default;

        //Open sensor
        virtual bool open() = 0;

        //Close sensor
        virtual void close() = 0;

        //Read temparature
        //returns true if temperature successfully was read and placed in t
        virtual bool getTemperature(double &t) = 0;

        virtual void setParameter(ParameterType paramtype, int param) = 0;
};
}

#endif
