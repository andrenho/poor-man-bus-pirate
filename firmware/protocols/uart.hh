#ifndef UART_HH_
#define UART_HH_

#include <hardware/uart.h>

#include "protocol.hh"

class UART : public Protocol {
public:
    explicit UART(uart_inst_t *uart_id);

    void disable();

private:
    uart_inst_t* uart_id_;

    uint32_t       baud_rate_ = 115200;
    bool           cts_       = false;
    bool           rts_       = false;
    uint8_t        data_bits_ = 8;
    uint8_t        stop_bits_ = 1;
    uart_parity_t  parity_    = UART_PARITY_NONE;

    const uint8_t  tx_pin_, rx_pin_;
};

#endif //UART_HH_
