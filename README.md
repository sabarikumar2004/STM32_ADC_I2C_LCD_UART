# STM32F103 Bare-Metal ADC, I2C LCD & UART

![STM32 ADC I2C LCD UART Project](Image/project_thumbnail.png)

## Project Overview

This project demonstrates **ADC, I2C, LCD, and UART communication** using the STM32F103C8T6 Blue Pill.

The project is developed using **bare-metal C programming** by directly accessing STM32 peripheral registers without using HAL libraries.

A potentiometer is connected to the ADC input. The ADC value is converted into voltage and displayed on a 16x2 LCD through I2C. The same voltage value is also sent through UART to a PC terminal.

## Features

* STM32F103C8T6 Blue Pill
* Bare-metal / register-level programming
* 12-bit ADC
* Potentiometer voltage measurement
* 16x2 LCD with I2C PCF8574 backpack
* UART serial communication
* Voltage display in volts
* UART output using PuTTY

## Hardware Used

* STM32F103C8T6 Blue Pill
* Potentiometer
* 16x2 LCD
* PCF8574 I2C LCD module
* USB-to-TTL converter
* ST-Link V2
* Soldering

## Pin Connections

### Potentiometer

| Potentiometer | STM32 |
| ------------- | ----- |
| VCC           | 3.3V  |
| GND           | GND   |
| Wiper         | PA0   |

PA0 is connected to **ADC1 Channel 0**.

### I2C LCD

| LCD / PCF8574 | STM32 |
| ------------- | ----- |
| SDA           | PB7   |
| SCL           | PB6   |
| VCC           | 5V    |
| GND           | GND   |

### UART

| USB-to-TTL | STM32 |
| ---------- | ----- |
| RX         | PA9   |
| GND        | GND   |

USART1 TX is available on **PA9**.

## Peripherals Used

### ADC

* ADC Peripheral: ADC1
* ADC Channel: Channel 0
* Input Pin: PA0
* Resolution: 12-bit
* ADC Range: 0 to 4095
* Reference Voltage: 3.3V

The voltage is calculated using:

```text
Voltage = ADC_Value × 3.3 / 4095
```

### I2C

* I2C Peripheral: I2C1
* SCL: PB6
* SDA: PB7
* LCD I2C Address: 0x27
* I2C Speed: 100 kHz

### UART

* USART Peripheral: USART1
* TX: PA9
* Baud Rate: 9600
* Data: 8-bit
* Stop Bit: 1
* Parity: None

## LCD Output

The LCD displays:

```text
ADC_POT_VALUE:
1.65
```

The voltage changes according to the potentiometer position.

## UART Output

The same voltage is transmitted through USART1 and can be viewed using PuTTY.

Example:

```text
ADC_POT_VALUE: 1.65
ADC_POT_VALUE: 2.31
ADC_POT_VALUE: 3.02
```

## Software Tools

* STM32CubeIDE
* C Programming
* ST-Link
* PuTTY
* Proteus (for simulation/testing)

## Programming Approach

The project uses direct register access instead of HAL functions.

The following peripherals are configured through STM32 registers:

* RCC
* GPIO
* ADC
* I2C
* USART

This project helps in understanding STM32 peripheral registers, bit manipulation, GPIO configuration, ADC conversion, I2C communication, and UART transmission.

## Project Structure

STM32_ADC_I2C_LCD_UART
│
├── Image
│   └── project_thumbnail.png
│
├── Src
├── Startup
├── README.md
├── .project
├── .cproject
├── .gitignore
└── STM32F103C8TX_FLASH.ld

## Result

The potentiometer voltage is successfully measured using the STM32 ADC and displayed on:

1. 16x2 I2C LCD
2. PC terminal through UART

## Author

**K Sabari**

GitHub: https://github.com/sabarikumar2004

LinkedIn: https://www.linkedin.com/in/sabarikumar48
