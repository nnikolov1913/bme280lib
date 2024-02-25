
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
    NickSensors *sensor1 = new NickSensors(IDrukSensor::FAKE_SENSOR);
    while(count++ < 5) {
        sensor->getTemperature(temp);
        sensor1->getTemperature(temp1);
        std::cout << "Temperature " << temp << "ᵒC" <<  " fake temp " << temp1 << "ᵒC" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    delete sensor;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sensor = new NickSensors(IDrukSensor::BME280_INT_I2C);
    //sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, alarm_on_fire, NULL);
    sensor->setThreshold(21.0, IDrukSensor::ABOVE_VALUE, [](void *data, double t){ std::cout << "Temperature alarm " << t << "ᵒC" << std::endl;}, NULL);
    sensor1->setThreshold(20.0, IDrukSensor::BELOW_VALUE, [](void *data, double t){ std::cout << "Fake temperature alarm " << t << "ᵒC" << std::endl;}, NULL);
    std::this_thread::sleep_for(std::chrono::seconds(60));
    delete sensor;
    delete sensor1;
}
