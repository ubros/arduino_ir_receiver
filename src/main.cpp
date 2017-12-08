//
// Created by ryo on 2017/12/08.
//
#include "Arduino.h"

#include "IRReceiver/IRReceiver.h"

IRReceiver IRReceiver;

void setup() {
    Serial.begin(115200);
    IRReceiver.begin(8);
}

void loop() {

    if (Serial.read() != -1) {
        Serial.println("スキャン開始");
        IRReceiver.scan();
    }

    IRReceiver.handler();
}

void IRReceiver::CALLBACK(uint8_t state, byte *signal, long length) {
    switch (state) {
        case IRReceiver::SUCCESS:
            Serial.println("受信成功:");
            for (long i = 0; i < length; i++) {
                if (i % 2 != 0) {
                    Serial.print(makeWord(signal[i], signal[i + 1]));
                    Serial.print(',');
                }
            }
            Serial.print('\n');
            break;
        case IRReceiver::FAILURE:
            Serial.print("受信失敗:");
            Serial.println(micros());
            break;
        default:
            break;
    }
}