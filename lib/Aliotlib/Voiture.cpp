#ifndef ALIOTLIB_VOITURE_H
#define ALIOTLIB_VOITURE_H
#include <Arduino.h>

#define for_i(x, i) for (byte i = 0; i < sizeof(x); i++)

struct Voiture
{
    // byte pinFLA, pinLB, pinFRA, pinFRB, pinBLA, pinBLB, pinBRA, pinBRB;
    byte pins[8];

    Voiture(byte pins[8])
    {
    }

    void init(void)
    {
        for_i(pins, i)
            pinMode(pins[i], OUTPUT);
    }

    void forward(void)
    {
        for_i(pins, i)
            digitalWrite(pins[i], i % 2 == 0 ? HIGH : LOW);
    }

    void backward(void)
    {
        for_i(pins, i)
            digitalWrite(pins[i], i % 2 == 0 ? LOW : HIGH);
    }

    // TODO left and right (I won't do it, ugh gnnnnn)
    void turnRight(void)
    {
        digitalWrite(pins[0], HIGH);
        digitalWrite(pins[1], LOW);

        digitalWrite(pins[4], HIGH);
        digitalWrite(pins[5], LOW);

        digitalWrite(pins[2], LOW);
        digitalWrite(pins[3], HIGH);

        digitalWrite(pins[6], LOW);
        digitalWrite(pins[7], HIGH);
    }

    void turnLeft(void)
    {
        digitalWrite(pins[0], LOW);
        digitalWrite(pins[1], HIGH);

        digitalWrite(pins[4], LOW);
        digitalWrite(pins[5], HIGH);

        digitalWrite(pins[2], HIGH);
        digitalWrite(pins[3], LOW);

        digitalWrite(pins[6], HIGH);
        digitalWrite(pins[7], LOW);
    }
};

#endif