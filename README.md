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
