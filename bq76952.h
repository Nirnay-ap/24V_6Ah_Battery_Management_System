#ifndef BQ76952_H
#define BQ76952_H

#include "i2c_hal.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t CELL_OV : 1;
  uint8_t CELL_UV : 1;
} bq76952_protection_t;

void bq76952_init(I2C_HandleTypeDef *hi2c);
int bq76952_getCellVoltage(uint8_t cell);
void bq76952_getAllCellVoltages(int *voltages);
int bq76952_getCurrent(void);
int bq76952_getInternalTemp(void); // Changed from float to int
bq76952_protection_t bq76952_getProtectionStatus(void);
bool bq76952_isConnected(void);
unsigned int bq76952_getDeviceNumber(void);
uint8_t bq76952_HandleAlarm(void);

#endif /* BQ76952_H */
