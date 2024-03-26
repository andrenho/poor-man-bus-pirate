# Poor man's Bus Pirate
A very basic bus sniffer and communicator (UART, SPI, I2C, etc). A simplified (and cheaper) version of the [Bus Pirate](https://dangerousprototypes.com/docs/Bus_Pirate).

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/6b8cda69-5a8c-4264-a6e0-ec4868ee3aef)

## Features

- Connect to PC via USB (USB also powers the circuit)
- Protocols supported: UART, SPI (master, slave, sniffing), I2C, PWM
- LEDs for all communication pins

## Pinout

| BP pin | UART | SPI master | SPI slave | SPI sniff | I2C function | PWM function | RPi Pico pin       |
|--------|------|------------|-----------|-----------|--------------|--------------|--------------------|
| COMM0  | TX   | MISO       | MOSI      | RX0       | SDA          | PWM0         | GP0 (1)            |
| COMM1  | RX0  | CS         | SS        | SS        | SCL          |              | GP1 (2)            |
| COMM2  | CTS  | SCK        | SCK       | SCK       |              |              | GP2 (4), GP10 (14) |
| COMM3  | RTS  | MOSI       | MISO      |           |              |              | GP3 (5)            |
| COMM4  | RX1  |            |           | RX1       |              |              | GP5 (7), GP8 (11)  |
| GND    |      |            |           |           |              |              | GND                |

## Commands

- General
  - `use [ uart | spi_master | spi_slave | spi_sniff | i2c_master | i2c_slave | pwm0 | pwm1 ]
  - `reset` (reset all options to default)
  - `set` (print all variables)
- UART
  - `set uart.baud [NNN]` (default: 115200)
  - `set uart.mode [MMM]` (default: 8N1)
  - `set uart.hflow [on | off]` (default: off)
  - `set uart.output [ascii | dec | bin | hex]` (default: ascii)
- SPI
  - `set spi.cpol [0 | 1]` (default: 0)
  - `set spi.cpha [0 | 1]` (default: 0)
  - `set spi.order [lsb | msb]` (default: msb)
  - `set spi.baud [NNNN]` (baud rate in Hz, master only, default: 100000)
  - `set spi.autorespond [off | NN]` (always respond with byte NN, default off)
  - `set spi.output [ascii | dec | bin | hex]` (default: hex)
- I2C
  - `set i2c.baud [NNNN]` (baud rate in Hz, master only, default: 100000)
  - `set i2c.output [ascii | dec | bin | hex]` (default: hex)
- PWM
  - TODO

Options are saved on RPI, and persist after a reboot.
