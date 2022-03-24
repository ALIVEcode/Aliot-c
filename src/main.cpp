#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>

// Pin assignation
#define LED_Status 2
#define button 34
#define led 22

AliotObj ampoule("e8f599b2-2806-47c2-9c0c-266bb91f89a0");
AliotProject monProject("9cb50433-129e-4905-9144-366c31fade7c");

void setup()
{
    pinMode(LED_Status, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    WiFi.mode(WIFI_STA);

    Serial.begin(115200);

    // wm.resetSettings();

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
    else
    {
        JSON fields;
        fields["/document/LED"] = true;
        fields["/document/Lumiere"] = "green";

        monProject.updateDoc(fields);
    }
    delay(300);
}