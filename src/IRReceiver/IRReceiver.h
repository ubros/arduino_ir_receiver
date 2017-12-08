//
// Created by ryo on 2017/12/08.
//

#ifndef ARDUINO_IR_RECEIVE_IRRECEIVER_H
#define ARDUINO_IR_RECEIVE_IRRECEIVER_H

#include "Arduino.h"

#define TIMEOUT_RECV_NOSIGNAL  (50000)
#define TIMEOUT_RECV           (5000000)

class IRReceiver {

    enum {
        STANDBY = 0x00,
        INIT = 0x01,
        SCANNING = 0x02,
        SUCCESS = 0x10,
        FAILURE = 0x11,
    } STATUS;

    struct {
        uint8_t input_pin;
        uint8_t status;
        byte buf[1024];
        volatile byte pos;
    } data_t;

    struct {
        char pre_value;
        char now_value;
        bool wait_flag;
        long pre_us;
        long now_us;
        long index;
        long i;
    } scan_t;

public:
    void begin(uint8_t input_pin);

    void handler();

    void scan();

    void CALLBACK(uint8_t state, byte *signal, long length);

private:
    void _init();

    void _scan();
};


#endif //ARDUINO_IR_RECEIVE_IRRECEIVER_H
