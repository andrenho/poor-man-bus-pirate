# poor-man-bus-pirate
A very basic bus sniffer and manipulator (UART, SPI, I2C)

## Features

- Connect via USB or WiFi
- Powered by USB or own circuit (5v only?)
- Protocols supported: UART, SPI, I2C, parallel
    - Maybe - JTAG
- R/W or sniff
- User interfaces: tty, curses, ui
- Pins: (5V OR 3V3)
    - GND, VCC
    - MOSI, MISO, SCK, 3xSS
    - 2x RX,TX + USART
    - I2C
    - PWM
- LEDs for all pins (high and low?)

## Connections

| BP pin | UART function | SPI master | SPI slave | SPI sniff | I2C function | PWM function | RPi Pico pin |
|--------|---------------|------------|-----------|-----------|--------------|--------------|--------------|
| COMM0  | TX            | MISO       | MOSI      | RX0       | SDA          | PWM0         | GP0 (1)      |
| COMM1  | RX            | CS         | SS        | SS        | SCL          |              | GP1 (2)      |
| COMM2  | CTS           | SCK        | SCK       | SCK       |              | PMW1         | GP2 (4), GP10 (14) |
| COMM3  | RTS           | MOSI       | MISO      |           |              |              | GP3 (5)      |
| COMM4  |               |            |           | RX1       |              |              | GP8 (11)     |
