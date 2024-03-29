#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include <stdint.h>

void i2c_master_init(uint8_t addr);
void i2c_master_scan();

#endif //I2C_MASTER_H_
