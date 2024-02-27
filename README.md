# bme280lib

Small library to handle temperature sensors.

It handles bme280 through I2C interface. Source code from Bosch for this sensor is used to read the sensor.
It works on Raspbery Pi when sensor is connected to /dev/i2c-1. These are GPIO02(SDA1) and GPIO03(SCL1) on the PI header.

To compile just run:

make

library is named libnicksensors.so

Some packages might be needed for the compilation. For example libi2c0 and libi2c-dev.

There is a small example program in main.cpp which shows how it can be used.
The library also contains a fake sensor just for test purposes
The executable is called readsensors and can be started with the command:

LD_LIBRARY_PATH=. ./readsensors

