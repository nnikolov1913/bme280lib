#ifndef LINUX_USERSPACE_H
#define LINUX_USERSPACE_H

extern "C" {

//Initialize the sensor/device. Using I2C by default
int bme280_main(const char* devname, int address);

//Read data from the sensor. Using double by default
int bme280_read_data(struct bme280_data *comp_data);

//Close the sensor/device
void bme280_close();

}

#endif // LINUX_USERSPACE_H
