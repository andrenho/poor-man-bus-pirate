# Poor man's Bus Pirate

A very basic bus sniffer and communicator (UART, SPI, I2C, etc). A simplified (and cheaper) version of the [Bus Pirate](https://dangerousprototypes.com/docs/Bus_Pirate).

## Features

This circuit can be used to either sniff communication between components (such as microcontrollers), or to directly communicate with the components using the computer.

- Supported input/output formats: ASCII, hexadecimal, decimal and binary
- Supported protocols: 
  - **UART** (with or without hardware flow control)
    - Baud autodetect
  - **SPI** (master, slave, sniffing)
  - **I2C** (not implemented yet)
  - **PWM** output
- Supported I/O voltages: 3.3V and 5V

## Commands

Connect the Pico USB to the computer, and then open a serial terminal to the USB port at 115200 baud.

- General
  - `use [ uart | spi_master | spi_slave | spi_sniff | pwm ]`
  - `reset` (reset all options to default)
  - `set` (print all variables)
- UART
  - `set uart.baud [NNN]` (default: 115200)
  - `set uart.mode [MMM]` (default: 8N1)
  - `set uart.hflow [on | off]` (default: off)
  - `set uart.output [ascii | dec | bin | hex]` (default: ascii)
  - `guess uart`: tries to autodetect baudrate
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
  - `set pwm.freq [NNNN]` (frequency in Hz, default: 100000)
  - `set pwm.duty [NN]` (% duty cycle, default: 50)

Settings are saved on flash, and persist after a reboot.

Communication example:

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/18411b9d-3f05-4c4b-a21a-cb78f8430f43)

## Pinout

| BP pin     | UART | SPI master | SPI slave | SPI sniff | I2C function | PWM function | RPi Pico pin       |
|------------|------|------------|-----------|-----------|--------------|--------------|--------------------|
| **COMM0**  | TX   | MISO       | MOSI      | RX0       | SDA          | PWM0         | GP0 (1)            |
| **COMM1**  | RX0  | CS         | SS        | SS        | SCL          |              | GP1 (2)            |
| **COMM2**  | CTS  | SCK        | SCK       | SCK       |              |              | GP2 (4), GP10 (14) |
| **COMM3**  | RTS  | MOSI       | MISO      |           |              |              | GP3 (5)            |
| **COMM4**  | RX1  |            |           | RX1       |              |              | GP5 (7), GP8 (11)  |
| **GND**    |      |            |           |           |              |              | GND                |

# Circuit

This section is mandatory - it provides the computer interface, and allows communicating with components at 3.3V.

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/44ba63c5-f81d-4478-bd14-66dc913552da)

This section is optional, and allows for communicating with 5V devices as well.

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/5a567604-431e-45d2-851b-0d46df98cebb)

This section is optional, and adds blinking lights to communication channels to indicate activity.

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/0d1dda8b-4ede-4791-a77b-8b5e4be25a43)


# Implementation

This is the circuit I implemented for my personal use:

![image](https://github.com/andrenho/poor-man-bus-pirate/assets/84652/6b8cda69-5a8c-4264-a6e0-ec4868ee3aef)

