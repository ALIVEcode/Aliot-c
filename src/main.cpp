#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>
#include <Voiture.cpp>

// Pin assignation
#define LED_Status 2
#define button 34
#define RESET_BUTTON 33
#define led 22

#define MBLA 12 // directionnal pin

PWM_Pin mbla(MBLA);

AliotObj ampoule("e8f599b2-2806-47c2-9c0c-266bb91f89a0");
AliotProject monProject("9cb50433-129e-4905-9144-366c31fade7c");

byte pins[]{1, 2, 3, 4, 5, 6, 7, 8};

Voiture maVoiture(pins);

void setup()
{
    pinMode(LED_Status, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    pinMode(RESET_BUTTON, INPUT);

    WiFi.mode(WIFI_STA);

    Serial.begin(115200);

    bool connected = aliot::connectToWiFi();
    if (!connected)
        END_PROGRAM

    digitalWrite(LED_Status, HIGH);

    if (!aliot::connectToAliveCode())
        END_PROGRAM

    ampoule.connect();
}

bool msg = false;

void loop()
{
    if (!ampoule.update())
    {
        if (!msg)
        {
            msg = true;
            println("Disconnected from ALIVEcode");
        }
        return;
    }

    if (digitalRead(button) == LOW)
    {
        JSON fields;
        fields["/document/LED"] = false;
        fields["/document/Lumiere"] = "red";

        monProject.updateDoc(fields);
    }

    if (!aliot::resetWiFiOnPress(RESET_BUTTON))
        END_PROGRAM
}