#ifndef I2C_HAL_H
#define I2C_HAL_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define I2C_TIMEOUT_MS 1000

HAL_StatusTypeDef i2c_write(uint8_t dev_addr, uint8_t *data, uint16_t len);
HAL_StatusTypeDef i2c_read(uint8_t dev_addr, uint8_t *data, uint16_t len);
HAL_StatusTypeDef i2c_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);
HAL_StatusTypeDef i2c_read_reg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);

#endif /* I2C_HAL_H */
