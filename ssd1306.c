#include "ssd1306.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Simple font data (5x7 characters)
static const uint8_t font_5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    // Add more characters as needed...
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x3E, 0x41, 0x49, 0x49, 0x3A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x55, 0x2A, 0x55, 0x2A, 0x55, // backslash
    0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x0C, 0x52, 0x52, 0x52, 0x3E, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x04, 0x08, 0x10, 0x08, // ~
};

static void ssd1306_write_command(ssd1306_t *oled, uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};
    i2c_write(SSD1306_I2C_ADDR, data, 2);
}

static void ssd1306_write_data(ssd1306_t *oled, uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i += 31) {
        uint8_t chunk_size = (len - i) > 31 ? 31 : (len - i);
        uint8_t buffer[32];
        buffer[0] = 0x40;
        memcpy(&buffer[1], &data[i], chunk_size);
        i2c_write(SSD1306_I2C_ADDR, buffer, chunk_size + 1);
    }
}

void ssd1306_init(ssd1306_t *oled, I2C_HandleTypeDef *hi2c)
{
    oled->hi2c = hi2c;

    uint8_t init_cmds[] = {
        0xAE,        // Display OFF
        0x20, 0x00,  // Memory addressing mode: Horizontal
        0x21, 0, 127,// Column address range: 0-127
        0x22, 0, 7,  // Page address range: 0-7
        0x40,        // Display start line
        0x81, 0x7F,  // Contrast: 127
        0xA1,        // Segment remap
        0xA6,        // Normal display
        0xA8, 0x3F,  // Multiplex ratio: 64-1
        0xC8,        // COM output scan direction
        0xD3, 0x00,  // Display offset
        0xD5, 0x80,  // Oscillator frequency
        0xD9, 0x22,  // Pre-charge period
        0xDA, 0x12,  // COM pins hardware configuration
        0xDB, 0x20,  // VCOMH deselect level
        0x8D, 0x14,  // Charge pump enable
        0xAF         // Display ON
    };

    for (uint16_t i = 0; i < sizeof(init_cmds); i++) {
        ssd1306_write_command(oled, init_cmds[i]);
        HAL_Delay(1);
    }

    ssd1306_clear(oled);
#ifdef __SIMULATOR__
    printf("SSD1306 initialized\n");
#endif
}

void ssd1306_clear(ssd1306_t *oled)
{
    memset(oled->buffer, 0, SSD1306_BUFFER_SIZE);
#ifndef __SIMULATOR__
    ssd1306_write_command(oled, 0x21);
    ssd1306_write_command(oled, 0);
    ssd1306_write_command(oled, 127);
    ssd1306_write_command(oled, 0x22);
    ssd1306_write_command(oled, 0);
    ssd1306_write_command(oled, 7);
    ssd1306_write_data(oled, oled->buffer, SSD1306_BUFFER_SIZE);
#endif
}

// Helper function to draw a character
static void ssd1306_draw_char(ssd1306_t *oled, uint8_t x, uint8_t y, char c)
{
    if (c < 32 || c > 126) return; // Only printable characters

    uint8_t *font_ptr = (uint8_t*)&font_5x7[(c - 32) * 5];

    for (uint8_t i = 0; i < 5; i++) {
        if (x + i < SSD1306_WIDTH) {
            oled->buffer[x + i + (y / 8) * SSD1306_WIDTH] = font_ptr[i];
        }
    }
}

// Helper function to draw a string
static void ssd1306_draw_string(ssd1306_t *oled, uint8_t x, uint8_t y, const char *str)
{
    while (*str) {
        ssd1306_draw_char(oled, x, y, *str++);
        x += 6; // 5 pixels for char + 1 pixel spacing
        if (x >= SSD1306_WIDTH - 6) {
            x = 0;
            y += 8; // Move to next line
            if (y >= SSD1306_HEIGHT) break;
        }
    }
}

// Helper function to draw numbers
static void ssd1306_draw_number(ssd1306_t *oled, uint8_t x, uint8_t y, int num)
{
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", num);
    ssd1306_draw_string(oled, x, y, buffer);
}

void ssd1306_update_bms_data(ssd1306_t *oled, int *cell_voltages, int current, int temp, int soc, bq76952_protection_t prot, uint8_t alert)
{
#ifdef __SIMULATOR__
    printf("=== OLED Display ===\n");
    printf("C1:%dmV C2:%dmV\n", cell_voltages[0], cell_voltages[1]);
    printf("C3:%dmV C4:%dmV\n", cell_voltages[2], cell_voltages[3]);
    printf("I:%dmA T:%dC\n", current, temp);
    printf("SoC:%d%%\n", soc);
    if (alert || prot.CELL_OV || prot.CELL_UV) {
        printf("STATUS: ALERT");
        if (prot.CELL_OV) printf(" OV");
        if (prot.CELL_UV) printf(" UV");
        printf("\n");
    } else {
        printf("STATUS: OK\n");
    }
    printf("==================\n");
#else
    ssd1306_clear(oled);

    // Display BMS data on OLED
    ssd1306_draw_string(oled, 0, 0, "BMS Monitor");

    // Cell voltages
    char cell_str[20];
    snprintf(cell_str, sizeof(cell_str), "C1:%d C2:%d", cell_voltages[0], cell_voltages[1]);
    ssd1306_draw_string(oled, 0, 8, cell_str);

    snprintf(cell_str, sizeof(cell_str), "C3:%d C4:%d", cell_voltages[2], cell_voltages[3]);
    ssd1306_draw_string(oled, 0, 16, cell_str);

    // Current and Temperature
    snprintf(cell_str, sizeof(cell_str), "I:%dmA T:%dC", current, temp);
    ssd1306_draw_string(oled, 0, 24, cell_str);

    // SoC
    snprintf(cell_str, sizeof(cell_str), "SoC: %d%%", soc);
    ssd1306_draw_string(oled, 0, 32, cell_str);

    // Status
    if (alert || prot.CELL_OV || prot.CELL_UV) {
        ssd1306_draw_string(oled, 0, 40, "STATUS: ALERT");
        if (prot.CELL_OV) ssd1306_draw_string(oled, 0, 48, "OV Protection!");
        else if (prot.CELL_UV) ssd1306_draw_string(oled, 0, 48, "UV Protection!");
        else ssd1306_draw_string(oled, 0, 48, "Alert Active!");
    } else {
        ssd1306_draw_string(oled, 0, 40, "STATUS: OK");
    }

    // Update display
    ssd1306_write_command(oled, 0x21);
    ssd1306_write_command(oled, 0);
    ssd1306_write_command(oled, 127);
    ssd1306_write_command(oled, 0x22);
    ssd1306_write_command(oled, 0);
    ssd1306_write_command(oled, 7);
    ssd1306_write_data(oled, oled->buffer, SSD1306_BUFFER_SIZE);
#endif
}
