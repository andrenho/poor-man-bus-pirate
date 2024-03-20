# poor-man-bus-pirate
A very basic bus sniffer and communicator (UART, SPI, I2C, etc). A simplified (and cheaper) version of Bus Pirate.

## Features

- Connect to PC via USB (USB also powers the circuit)
- Protocols supported: UART, SPI (master, slave, sniffing), I2C, PWM
- LEDs for all communication pins

## Pinout

| BP pin | UART function | SPI master | SPI slave | SPI sniff | I2C function | PWM function | RPi Pico pin |
|--------|---------------|------------|-----------|-----------|--------------|--------------|--------------|
| COMM0  | TX            | MISO       | MOSI      | RX0       | SDA          | PWM0         | GP0 (1)      |
| COMM1  | RX            | CS         | SS        | SS        | SCL          |              | GP1 (2)      |
| COMM2  | CTS           | SCK        | SCK       | SCK       |              | PMW1         | GP2 (4), GP10 (14) |
| COMM3  | RTS           | MOSI       | MISO      |           |              |              | GP3 (5)      |
| COMM4  |               |            |           | RX1       |              |              | GP8 (11)     |
| GND    |               |            |           |           |              |              | GND          |

## Commands

- General
  - `use [ hiz | uart | spi_master | spi_slave | spi_sniff | i2c_master | i2c_slave | pwm0 | pwm1 ]
  - `reset` (reset all options to default)
- UART
  - `set uart.baud [NNN]` (default: 115200)
  - `set uart.mode [MMM]` (default: 8N1)
  - `set uart.hflow [on | off]` (default: off)
- SPI
  - `set spi.cpol [0 | 1]` (default: 0)
  - `set spi.cpha [0 | 1]` (default: 0)
  - `set spi.order [lsb | msb]` (default: msb)
  - `set spi.baud [NNNN]` (baud rate in Hz, master only, default: 100000)
  - `set spi.autorespond [off | NN]` (always respond with byte NN, default off)
- I2C
  - `set i2c.baud [NNNN]` (baud rate in Hz, master only, default: 100000)
- PWM
  - TODO

Options are saved on RPI, and persist after a reboot.