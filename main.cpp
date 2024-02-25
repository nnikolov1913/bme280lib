
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
    double temp;
    NickSensors *sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    while(count++ < 5) {
        sensor->getTemperature(temp);
        std::cout << "Temperature " << temp << "ᵒC" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    delete sensor;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    //sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, alarm_on_fire, NULL);
    sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, [](void *data, double t){ std::cout << "Temperature alarm " << t << "ᵒC" << std::endl;}, NULL);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    delete sensor;
}
