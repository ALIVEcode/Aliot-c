#ifndef ALIOTLIB_VOITURE_H
#define ALIOTLIB_VOITURE_H
#include <Arduino.h>

#define for_i(x, i) for (byte i = 0; i < sizeof(x); i++)

struct Voiture
{
    // byte pinFLA, pinFLB, pinFRA, pinFRB, pinBLA, pinBLB, pinBRA, pinBRB;
    byte pins[8];

    Voiture(byte pins[8])
    {
        for_i(pins, i)
            pinMode(pins[i], OUTPUT);
    }

    void forward()
    {
        for_i(pins, i)
            digitalWrite(pins[i], i % 2 == 0 ? HIGH : LOW);
    }

    void backward()
    {
        for_i(pins, i)
            digitalWrite(pins[i], i % 2 == 0 ? LOW : HIGH);
    }

    // TODO left and right (I won't do it, ugh gnnnnn)
};

#endif