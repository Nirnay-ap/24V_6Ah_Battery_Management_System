#include "bq76952.h"
#include <stdio.h>
#include <stdlib.h>

static I2C_HandleTypeDef *bq_i2c;
#define BQ76952_I2C_ADDR 0x10

void bq76952_init(I2C_HandleTypeDef *hi2c)
{
  bq_i2c = hi2c;
#ifdef __SIMULATOR__
  printf("BQ76952 initialized in simulation mode\n");
#else
  uint8_t init_cmd[] = {0x00, 0x01};
  i2c_write_reg(BQ76952_I2C_ADDR, 0x00, init_cmd, 2);
  printf("BQ76952 hardware initialized\n");
#endif
}

int bq76952_getCellVoltage(uint8_t cell)
{
#ifdef __SIMULATOR__
  int voltages[] = {3700, 3650, 3720, 3680};
  return (cell < 4) ? voltages[cell] + (rand() % 20 - 10) : 0;
#else
  uint8_t reg = 0x14 + cell * 2;
  uint8_t data[2];
  if (i2c_read_reg(BQ76952_I2C_ADDR, reg, data, 2) == HAL_OK) {
    return ((data[0] << 8) | data[1]);
  }
  return 0;
#endif
}

void bq76952_getAllCellVoltages(int *voltages)
{
#ifdef __SIMULATOR__
  int sim_voltages[] = {3700, 3650, 3720, 3680};
  for (int i = 0; i < 4; i++) {
    voltages[i] = sim_voltages[i] + (rand() % 20 - 10);
  }
#else
  for (int i = 0; i < 4; i++) {
    voltages[i] = bq76952_getCellVoltage(i);
  }
#endif
}

int bq76952_getCurrent(void)
{
#ifdef __SIMULATOR__
  return 5000 + (rand() % 100 - 50);
#else
  uint8_t data[2];
  if (i2c_read_reg(BQ76952_I2C_ADDR, 0x3A, data, 2) == HAL_OK) {
    return ((data[0] << 8) | data[1]);
  }
  return 0;
#endif
}

// Change return type to int instead of float
int bq76952_getInternalTemp(void)
{
#ifdef __SIMULATOR__
  return 250 + (rand() % 20 - 10); // Return as integer (25.0°C = 250)
#else
  uint8_t data[2];
  if (i2c_read_reg(BQ76952_I2C_ADDR, 0x68, data, 2) == HAL_OK) {
    return ((data[0] << 8) | data[1]); // Return raw value, let caller scale
  }
  return 0;
#endif
}

bq76952_protection_t bq76952_getProtectionStatus(void)
{
  bq76952_protection_t status = {0};
#ifdef __SIMULATOR__
  status.CELL_OV = (rand() % 50 == 0);
  status.CELL_UV = (rand() % 50 == 0);
#else
  uint8_t data;
  if (i2c_read_reg(BQ76952_I2C_ADDR, 0x02, &data, 1) == HAL_OK) {
    status.CELL_OV = (data & 0x01) ? 1 : 0;
    status.CELL_UV = (data & 0x02) ? 1 : 0;
  }
#endif
  return status;
}

bool bq76952_isConnected(void)
{
#ifdef __SIMULATOR__
  return true;
#else
  uint8_t data;
  return (i2c_read_reg(BQ76952_I2C_ADDR, 0x00, &data, 1) == HAL_OK);
#endif
}

unsigned int bq76952_getDeviceNumber(void)
{
#ifdef __SIMULATOR__
  return 0x76952;
#else
  uint8_t data[2];
  if (i2c_read_reg(BQ76952_I2C_ADDR, 0x01, data, 2) == HAL_OK) {
    return ((data[0] << 8) | data[1]);
  }
  return 0;
#endif
}

uint8_t bq76952_HandleAlarm(void)
{
#ifdef __SIMULATOR__
  return (rand() % 256);
#else
  uint8_t data;
  if (i2c_read_reg(BQ76952_I2C_ADDR, 0x04, &data, 1) == HAL_OK) {
    return data;
  }
  return 0;
#endif
}
