void ssd1306_update_bms_data(ssd1306_t *oled, int *cell_voltages, int current, int temp, int soc, bq76952_protection_t prot, uint8_t alert)
{
#ifdef __SIMULATOR__
    printf("=== OLED Display ===\n");
    printf("C1:%dmV C2:%dmV C3:%dmV\n", cell_voltages[0], cell_voltages[1], cell_voltages[2]);
    printf("C4:%dmV C5:%dmV C6:%dmV\n", cell_voltages[3], cell_voltages[4], cell_voltages[5]);
    printf("C7:%dmV\n", cell_voltages[6]);
    printf("I:%dmA T:%dC SoC:%d%%\n", current, temp, soc);
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
    ssd1306_draw_string(oled, 0, 0, "BMS Monitor 7-Cell");

    // Cell voltages - Display in 3 rows to fit all 7 cells
    char cell_str[30];
    
    // Row 1: Cells 1-3
    snprintf(cell_str, sizeof(cell_str), "C1:%d C2:%d C3:%d", 
             cell_voltages[0], cell_voltages[1], cell_voltages[2]);
    ssd1306_draw_string(oled, 0, 8, cell_str);

    // Row 2: Cells 4-6  
    snprintf(cell_str, sizeof(cell_str), "C4:%d C5:%d C6:%d", 
             cell_voltages[3], cell_voltages[4], cell_voltages[5]);
    ssd1306_draw_string(oled, 0, 16, cell_str);

    // Row 3: Cell 7 and spacing
    snprintf(cell_str, sizeof(cell_str), "C7:%d", cell_voltages[6]);
    ssd1306_draw_string(oled, 0, 24, cell_str);

    // Current, Temperature and SoC
    snprintf(cell_str, sizeof(cell_str), "I:%dmA T:%dC", current, temp);
    ssd1306_draw_string(oled, 0, 32, cell_str);

    snprintf(cell_str, sizeof(cell_str), "SoC: %d%%", soc);
    ssd1306_draw_string(oled, 0, 40, cell_str);

    // Status - More compact display
    if (alert || prot.CELL_OV || prot.CELL_UV) {
        ssd1306_draw_string(oled, 0, 48, "ALERT:");
        if (prot.CELL_OV) 
            ssd1306_draw_string(oled, 40, 48, "OV");
        else if (prot.CELL_UV) 
            ssd1306_draw_string(oled, 40, 48, "UV");
        else 
            ssd1306_draw_string(oled, 40, 48, "ACTIVE");
    } else {
        ssd1306_draw_string(oled, 0, 48, "STATUS: OK");
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
