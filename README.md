# bme280lib

Small library to handle temperature sensors.

It handles bme280 through I2C interface. It works on Raspbery Pi when sensor is connected to /dev/i2c-1

To compile just run:

make

library is named libnicksensors.so

Some packages might be needed for the compilation. For example libi2c0 and libi2c-dev.

There is a small example program in main.cpp which shows how it can be used.
