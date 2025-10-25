void StartDefaultTask(void *argument)
{
  int loop_count = 0;

  printf("BMS Monitoring Started\n");
  printf("Time(s),Cell1(mV),Cell2(mV),Cell3(mV),Cell4(mV),Cell5(mV),Cell6(mV),Cell7(mV),Current(mA),Temp(C),SoC%%,Alert\n");

  for(;;)
  {
    int cellArray[7] = {0};  // Changed to 7 cells
    bool isConnected = bq76952_isConnected();
    unsigned int deviceNumber = bq76952_getDeviceNumber();

    // Read all BMS data
    bq76952_getAllCellVoltages(cellArray);
    int current = bq76952_getCurrent();
    int temp_int = (int)bq76952_getInternalTemp();
    bq76952_protection_t protStatus = bq76952_getProtectionStatus();

    // Calculate State of Charge (integer math only)
    int avg_voltage = 0;
    for (int i = 0; i < 7; i++) {  // Changed to 7 cells
      avg_voltage += cellArray[i];
    }
    avg_voltage /= 7;  // Changed to 7 cells

    int soc = 0;
    if (avg_voltage >= 3000 && avg_voltage <= 4200) {
      soc = ((avg_voltage - 3000) * 100) / 1200;
    }
    if (soc > 100) soc = 100;
    if (soc < 0) soc = 0;

    // Check alert pin
    uint8_t alert_triggered = 0;
#ifdef __SIMULATOR__
    alert_triggered = (rand() % 20 == 0) ? 1 : 0;
#else
    alert_triggered = (HAL_GPIO_ReadPin(ALERT_PORT, ALERT_PIN) == GPIO_PIN_SET) ? 1 : 0;
#endif

    // Handle alerts
    if (alert_triggered) {
      uint8_t alert_status = bq76952_HandleAlarm();
      printf("!ALERT! Status: 0x%02X\n", alert_status);
    }

    // Update OLED display (you may need to update the OLED function too)
    ssd1306_update_bms_data(&oled, cellArray, current, (float)temp_int, (float)soc, protStatus, alert_triggered);

    // CSV output
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",  // Added more fields
           loop_count,
           cellArray[0], cellArray[1], cellArray[2], cellArray[3], cellArray[4], cellArray[5], cellArray[6],  // 7 cells
           current, temp_int, soc, alert_triggered);

    // Status output
    printf("Device: 0x%X, Connected: %s\n",
           deviceNumber, isConnected ? "Yes" : "No");
    printf("Cells: %dmV %dmV %dmV %dmV %dmV %dmV %dmV\n",  // 7 cells
           cellArray[0], cellArray[1], cellArray[2], cellArray[3], cellArray[4], cellArray[5], cellArray[6]);
    printf("Current: %dmA, Temp: %dC, SoC: %d%%\n", current, temp_int, soc);

    if (protStatus.CELL_OV) printf("OV Protection!\n");
    if (protStatus.CELL_UV) printf("UV Protection!\n");

    printf("---\n");

    loop_count++;
    osDelay(1000);
  }
}
