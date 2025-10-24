🔋 24V 6Ah Battery Management System

<div id="header" align="center"> <img src="bms_customboard.png" width="850"/> </div>

🔋 BMS Firmware for STM32 & BQ76952  ⚡ STM32 | 🔧 Embedded C | 📊 Real-time Monitoring | 

📋 Project Overview
A sophisticated 24V 6Ah Battery Management System using STM32F103C8T6 microcontroller and Texas Instruments BQ76952 . Provides comprehensive battery monitoring, protection, and management for 8-series lithium-phosphate cells.

🎓 About the Project
This repository contains the firmware for a 24V 6Ah Battery Management System (BMS) built using the BQ76952 battery monitor IC and STM32F1 microcontroller. The system monitors cell voltages, current, temperature, and protection status, calculates State of Charge (SoC), handles alerts, and displays data on an SSD1306 OLED screen. It supports hardware mode and a simulation mode for development without physical hardware. The project uses FreeRTOS for task management and HAL libraries for peripheral control.

🚀 Project Details

🔋 Overview





Hardware Integration: BQ76952 for battery monitoring, SSD1306 OLED for display, STM32F1 for control.



Features:





Monitors 4 cell voltages, current, internal temperature.



Calculates SoC using average cell voltage (integer-based for efficiency).



Checks protection status (OV/UV) and handles alerts via GPIO pin.



Updates OLED display with BMS data and status.



Logs data in CSV format via UART for analysis.



Simulation mode (__SIMULATOR__) for testing without hardware.



Date: April 2025

📁 Code Structure





main.c: System initialization, FreeRTOS task for monitoring and display.



bq76952.c: Driver for BQ76952 (voltage, current, temp, protection).



ssd1306.c: OLED driver with text drawing and BMS data update.



i2c_hal.c: I2C wrappers using HAL.



stm32f1xx_hal_msp.c: MSP initialization for peripherals.



freertos.c: FreeRTOS application code.



Other files: Interrupt handlers, timebase, system config.
