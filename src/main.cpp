#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>

// Pin assignation
#define LED_Status 2
#define button 34
#define led 22

AliotObj ampoule{"e8f599b2-2806-47c2-9c0c-266bb91f89a0"};
AliotProjet monProjet{"9cb50433-129e-4905-9144-366c31fade7c"};

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
        ALIOT_FAIL

    digitalWrite(LED_Status, HIGH);

    if (!aliot::connectToAliveCode())
        ALIOT_FAIL

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
            Serial.println("Disconnected from ALIVEcode");
        }
        return;
    }

    if (digitalRead(button) == LOW)
    {
        JSON fields;
        fields["/document/LED"] = false;
        fields["/document/Lumiere"] = "red";

        monProjet.updateDoc(fields);
    }
    else
    {
        JSON fields;
        fields["/document/LED"] = true;
        fields["/document/Lumiere"] = "green";

        monProjet.updateDoc(fields);
    }
    delay(300);
}