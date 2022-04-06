#include <Arduino.h>

uint8_t i;
uint8_t channel[4];
uint8_t publicEnablePins[4];

struct Robot
{
    uint8_t enablePins[4];
    uint8_t directionalPins[8];
    unsigned int freq;
    uint8_t res;

    Robot(uint8_t enablePins[], uint8_t directionalPins[], unsigned int freq = 1000, uint8_t res = 8)
    {
        for (i = 0; i < 4; i++)
        {
            if (enablePins[i] <= 0)
                break;
            this->enablePins[i] = enablePins[i];
        }
        for (i = 0; i < 8; i++)
        {
            if (directionalPins[i] <= 0)
                break;
            this->directionalPins[i] = directionalPins[i];
        }
        this->freq = freq;
        this->res = res;
    }

    void init(void)
    {
        for (i = 0; i < 4; i++)
        {
            publicEnablePins[i] = this->enablePins[i];
            channel[i] = i;
            ledcSetup(channel[i], this->freq, this->res);
            ledcAttachPin(this->enablePins[i], channel[i]);
            pinMode(this->enablePins[i], OUTPUT);
        }
        for (i = 0; i < sizeof(this->directionalPins); i++)
            pinMode(this->directionalPins[i], OUTPUT);
    }

    void forward(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        for (i = 0; i < sizeof(this->directionalPins); i++)
        {
            digitalWrite(this->directionalPins[i], i % 2 == 0 ? HIGH : LOW);
        }
    }

    void backward(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        for (i = 0; i < sizeof(this->directionalPins); i++)
        {
            digitalWrite(this->directionalPins[i], i % 2 == 0 ? LOW : HIGH);
        }
    }

    void turnRight(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        digitalWrite(this->directionalPins[0], HIGH); // Top left CW
        digitalWrite(this->directionalPins[1], LOW);

        digitalWrite(this->directionalPins[2], LOW); // Top right CCW
        digitalWrite(this->directionalPins[3], HIGH);

        digitalWrite(this->directionalPins[4], HIGH); // Bottom left CW
        digitalWrite(this->directionalPins[5], LOW);

        digitalWrite(this->directionalPins[6], LOW); // Bottom right motor CCW
        digitalWrite(this->directionalPins[7], HIGH);
    }

    void turnLeft(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        digitalWrite(this->directionalPins[0], LOW); // Top left CCW
        digitalWrite(this->directionalPins[1], HIGH);

        digitalWrite(this->directionalPins[2], HIGH); // Top right CW
        digitalWrite(this->directionalPins[3], LOW);

        digitalWrite(this->directionalPins[4], LOW); // Bottom left CCW
        digitalWrite(this->directionalPins[5], HIGH);

        digitalWrite(this->directionalPins[6], HIGH); // Bottom right motor CW
        digitalWrite(this->directionalPins[7], LOW);
    }

    void strafeRight(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        digitalWrite(this->directionalPins[0], LOW); // Top left CCW
        digitalWrite(this->directionalPins[1], HIGH);

        digitalWrite(this->directionalPins[2], HIGH); // Top right CW
        digitalWrite(this->directionalPins[3], LOW);

        digitalWrite(this->directionalPins[4], HIGH); // Bottom left CCW
        digitalWrite(this->directionalPins[5], LOW);

        digitalWrite(this->directionalPins[6], LOW); // Bottom right motor CW
        digitalWrite(this->directionalPins[7], HIGH);
    }
    void strafeLeft(uint8_t value)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], value);
        digitalWrite(this->directionalPins[0], HIGH); // Top left CCW
        digitalWrite(this->directionalPins[1], LOW);

        digitalWrite(this->directionalPins[2], LOW); // Top right CW
        digitalWrite(this->directionalPins[3], HIGH);

        digitalWrite(this->directionalPins[4], LOW); // Bottom left CCW
        digitalWrite(this->directionalPins[5], HIGH);

        digitalWrite(this->directionalPins[6], HIGH); // Bottom right motor CW
        digitalWrite(this->directionalPins[7], LOW);
    }
    void stop(void)
    {
        for (i = 0; i < sizeof(this->enablePins); i++)
            ledcWrite(channel[i], 0);
    }
};

struct rangeSensor
{
    float duration, distance;
    uint8_t trigPin, echoPin;
    rangeSensor(uint8_t trigPin, uint8_t echoPin)
    {
        this->trigPin = trigPin;
        this->echoPin = echoPin;
    }
    void init(void)
    {
        pinMode(this->trigPin, OUTPUT);
        pinMode(this->echoPin, INPUT);
    }
    float read()
    {
        digitalWrite(this->trigPin, LOW);
        delayMicroseconds(10);
        digitalWrite(this->trigPin, HIGH);
        delayMicroseconds(10);

        this->duration = pulseIn(this->echoPin, HIGH);

        this->distance = (this->duration / 2) * 0.0343;

        if (distance >= 400)
        {
            this->distance = 400;
        }
        else if (distance <= 2)
        {
            this->distance = 15;
        }

        return this->distance;
    }
};
