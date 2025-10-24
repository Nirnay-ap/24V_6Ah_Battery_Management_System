#include "i2c_hal.h"

// External I2C handle - this should be defined in main.c
extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef i2c_write(uint8_t dev_addr, uint8_t *data, uint16_t len)
{
  return HAL_I2C_Master_Transmit(&hi2c1, dev_addr << 1, data, len, I2C_TIMEOUT_MS);
}

HAL_StatusTypeDef i2c_read(uint8_t dev_addr, uint8_t *data, uint16_t len)
{
  return HAL_I2C_Master_Receive(&hi2c1, dev_addr << 1, data, len, I2C_TIMEOUT_MS);
}

HAL_StatusTypeDef i2c_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
  uint8_t buffer[32];
  if (len > 31) return HAL_ERROR;

  buffer[0] = reg;
  for (uint16_t i = 0; i < len; i++) {
    buffer[i + 1] = data[i];
  }
  return HAL_I2C_Master_Transmit(&hi2c1, dev_addr << 1, buffer, len + 1, I2C_TIMEOUT_MS);
}

HAL_StatusTypeDef i2c_read_reg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, dev_addr << 1, &reg, 1, I2C_TIMEOUT_MS);
  if (status != HAL_OK) return status;
  return HAL_I2C_Master_Receive(&hi2c1, dev_addr << 1, data, len, I2C_TIMEOUT_MS);
}
