#ifndef ALIOTLIB_VOITURE_H
#define ALIOTLIB_VOITURE_H
#include <Arduino.h>

#define for_i(x, i) for (int i = 0; i < sizeof(x); i++)

enum Wheel
{
    FL_WHEEL = 0,
    FR_WHEEL = 2,
    BL_WHEEL = 4,
    BR_WHEEL = 6
};

enum Direction
{
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

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

    void turnWheel(Wheel wheel, Direction direction)
    {
        digitalWrite(pins[wheel], direction ? LOW : HIGH);
        digitalWrite(pins[wheel + 1], direction ? HIGH : LOW);
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
        turnWheel(Wheel::FL_WHEEL, Direction::CLOCKWISE);
        turnWheel(Wheel::BL_WHEEL, Direction::CLOCKWISE);
        turnWheel(Wheel::FR_WHEEL, Direction::COUNTER_CLOCKWISE);
        turnWheel(Wheel::BR_WHEEL, Direction::COUNTER_CLOCKWISE);
    }

    void turnLeft(void)
    {
        turnWheel(Wheel::FL_WHEEL, Direction::COUNTER_CLOCKWISE);
        turnWheel(Wheel::BL_WHEEL, Direction::COUNTER_CLOCKWISE);
        turnWheel(Wheel::FR_WHEEL, Direction::CLOCKWISE);
        turnWheel(Wheel::BR_WHEEL, Direction::CLOCKWISE);
    }
};

#endif