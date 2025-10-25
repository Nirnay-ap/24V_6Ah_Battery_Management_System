#ifndef SSD1306_H
#define SSD1306_H

#include "main.h"
#include "bq76952.h"

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t buffer[SSD1306_BUFFER_SIZE];
} ssd1306_t;

void ssd1306_init(ssd1306_t *oled, I2C_HandleTypeDef *hi2c);
void ssd1306_clear(ssd1306_t *oled);
void ssd1306_update_bms_data(ssd1306_t *oled, int *cell_voltages, int current, int temp, int soc, bq76952_protection_t prot, uint8_t alert);

#endif
