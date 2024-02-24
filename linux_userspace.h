#ifndef LINUX_USERSPACE_H
#define LINUX_USERSPACE_H

extern "C" {

int bme280_main(const char* devname);
int bme280_read_data(struct bme280_data *comp_data);
void bme280_close();

}

#endif // LINUX_USERSPACE_H
