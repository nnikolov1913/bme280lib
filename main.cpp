
#include <iostream>
#include <chrono>
#include <thread>
#include "linux_userspace.h"
#include "nicksenors.h"

using namespace druksensor;

/*
static void alarm_on_fire(void *data, double t) {
    std::cout << "Temperature alarm" << t << "ᵒC" << std::endl;
}
*/

int main(int argc, char *argv[])
{
    unsigned count = 0;
    double temp, temp1;

    NickSensors *sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    if (sensor && sensor->openSensor() == false) {
        std::cout << "BME280 sensor open ERROR " << std::endl;
        delete sensor;
        sensor = nullptr;
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

    sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    if (sensor->openSensor() == true) {
        //sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, alarm_on_fire, NULL);
        sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, [](void *data, double t)
                { std::cout << "Temperature alarm " << t << "ᵒC" << std::endl;}, NULL);
    }

    if (sensor1 != nullptr) {
        sensor1->setThreshold(20.0, IDrukSensor::BELOW_VALUE, [](void *data, double t)
                { std::cout << "Fake temperature alarm " << t << "ᵒC" << std::endl;}, NULL);
    }

    //Wait for some alarms. At least from fake sensor should come
    std::this_thread::sleep_for(std::chrono::seconds(60));

    sensor->removeThreshold();

    delete sensor;
    delete sensor1;
}
