/*
 * Example program to test libnicksensors.so
 */

#include <iostream>
#include <chrono>
#include <thread>
#include "linux_userspace.h"
#include "nicksenors.h"

using namespace druksensor;

class MySensorAlarm : public SensorAlarm {
public:
    MySensorAlarm(double t, IDrukSensor::ThresholdDir dir, IDrukSensor::SensorType type) : SensorAlarm(t, dir)
    {
        mType = type;
    }

    virtual void onAlarm(double t) override {
        std::cout << "Temperature alarm " << t << " sensor type " << (int)mType << std::endl;
    }
    IDrukSensor::SensorType mType;
};

int main(int argc, char *argv[])
{
    unsigned count = 0;
    double temp, temp1;

    NickSensors *sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    if (sensor) {
        sensor->setParameter(IDrukSensor::INTERFACE, 1);
        if (sensor->openSensor() == false) {
            std::cout << "BME280 sensor open ERROR " << std::endl;
            delete sensor;
            sensor = nullptr;
        }
    }

    NickSensors *sensor1 = new NickSensors(IDrukSensor::FAKE_SENSOR);
    if (sensor1 && sensor1->openSensor() == false) {
        std::cout << "Fake sensor open ERROR " << std::endl;
        delete sensor1;
        sensor1 = nullptr;
    }

    while(count++ < 5) {
        if (sensor != nullptr) {
            if (sensor->getTemperature(temp)) {
                std::cout << "BME280 temperature " << temp << " ᵒC" << std::endl;
            }
            else {
                std::cout << "Fake temperature ERROR" << std::endl;
            }
        }

        if (sensor1 != nullptr) {
            if (sensor1->getTemperature(temp1)) {
                std::cout << "Fake temperature " << temp1 << " ᵒC" << std::endl;
            }
            else {
                std::cout << "Fake temperature ERROR" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    delete sensor;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    MySensorAlarm alarmbme280(21.0, IDrukSensor::ABOVE_VALUE, IDrukSensor::BME280_INT_I2C);
    sensor = new NickSensors(alarmbme280.mType);
    if (sensor != nullptr && sensor->openSensor() == true) {
        sensor->setThreshold(&alarmbme280);
    }

    MySensorAlarm alarmfake(0.0, IDrukSensor::BELOW_VALUE, IDrukSensor::FAKE_SENSOR);
    MySensorAlarm alarmfake1(30.0, IDrukSensor::ABOVE_VALUE, IDrukSensor::FAKE_SENSOR);
    if (sensor1 != nullptr) {
        sensor1->setThreshold(&alarmfake);
        sensor1->setThreshold(&alarmfake1);
    }

    //Wait for some alarms. At least from fake sensor should come
    std::this_thread::sleep_for(std::chrono::seconds(60));

    sensor->removeThreshold(&alarmbme280);

    delete sensor;
    delete sensor1;
}
