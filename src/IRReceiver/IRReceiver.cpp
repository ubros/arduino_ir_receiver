//
// Created by ryo on 2017/12/08.
//

#include "IRReceiver.h"

void IRReceiver::begin(uint8_t input_pin) {
    Serial.println("IR Receiver");
    data_t.status = STANDBY;
    data_t.input_pin = input_pin;
    pinMode(data_t.input_pin, INPUT);
}

void IRReceiver::handler() {
    switch (data_t.status) {
        case STANDBY:
            break;
        case INIT:
            _init();
            break;
        case SCANNING:
            _scan();
            break;
        case SUCCESS:
            data_t.status = STANDBY;
            CALLBACK(SUCCESS, data_t.buf, scan_t.index);
            break;
        case FAILURE:
            data_t.status = STANDBY;
            CALLBACK(FAILURE, data_t.buf, scan_t.index);
            break;
        default:
            break;
    }
};

void IRReceiver::scan() {
    if (data_t.status == STANDBY)
        data_t.status = INIT;
}

void IRReceiver::_init() {
    data_t.pos = 0;
    data_t.status = SCANNING;

    scan_t.pre_value = HIGH;
    scan_t.now_value = HIGH;
    scan_t.wait_flag = true;
    scan_t.pre_us = micros();
    scan_t.now_us = 0;
    scan_t.index = 0;
    scan_t.i = 0;
}

void IRReceiver::_scan() {

    scan_t.now_value = digitalRead(data_t.input_pin);
    if (scan_t.pre_value != scan_t.now_value) {
        scan_t.now_us = micros();
        if (!scan_t.wait_flag) {
            short current = scan_t.now_us - scan_t.pre_us;
            data_t.buf[scan_t.index++] = lowByte(current);
            data_t.buf[scan_t.index++] = highByte(current);
        }
        scan_t.wait_flag = false;
        scan_t.pre_value = scan_t.now_value;
        scan_t.pre_us = scan_t.now_us;
    }

    if (scan_t.wait_flag) {
        if ((micros() - scan_t.pre_us) > TIMEOUT_RECV) {
            data_t.status = FAILURE;
        }
    } else {
        if ((micros() - scan_t.pre_us) > TIMEOUT_RECV_NOSIGNAL) {
            data_t.status = SUCCESS;
        }
    }

}