#ifndef LINUX_USERSPACE_H
#define LINUX_USERSPACE_H

extern "C" {

//Initialize the sensor/device. Using I2C by default
int bme280_main(const char* devname, int address, void **ctx);

//Read data from the sensor. Using double by default
int bme280_read_data(void *ctx, struct bme280_data *comp_data);

//Close the sensor/device
void bme280_close(void *ctx);

}

#endif // LINUX_USERSPACE_H
