#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>

// Pin assignation
#define LED_Status 2
#define button 34
#define led 22

void setup()
{
    pinMode(LED_Status, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    WiFi.mode(WIFI_STA);

    Serial.begin(115200);

    // wm.resetSettings();

    bool connected = aliot::connectToWiFi();
    while (!connected)
        ;

    digitalWrite(LED_Status, HIGH);
    AliotObj ampoule{"e8f599b2-2806-47c2-9c0c-266bb91f89a0"};

    if (aliot::connectToAliveCode())
        ampoule.connect();
    else
    {
        while (1)
        {
        }
    }
}

bool led_status = true;
void loop()
{
    if (digitalRead(button) == LOW)
    {
        String data;
        if (aliotClient.connected())
        {
            if (led_status)
            {
                data = String("{\"event\":\"update\",\"data\": {\"projectId\":\"50209e1b-1e1c-4df1-ad9a-3eaffdabed35\",\"fields\":{\"/document/led\": false}}}");
                aliotWebSocketClient.sendData(data);
                delay(2000);
                Serial.println("Data sent");
                aliotWebSocketClient.getData(data);
                while (!(data.length() > 0))
                {
                    aliotWebSocketClient.getData(data);
                }
                Serial.println(data);
                aliotClient.setTimeout(1);
                led_status = false;
            }
            else if (!led_status)
            {
                data = String("{\"event\":\"update\",\"data\": {\"projectId\":\"50209e1b-1e1c-4df1-ad9a-3eaffdabed35\",\"fields\":{\"/document/led\": true}}}");
                aliotWebSocketClient.sendData(data);
                delay(2000);
                Serial.println("Data sent");
                aliotWebSocketClient.getData(data);
                while (!(data.length() > 0))
                {
                    aliotWebSocketClient.getData(data);
                }
                Serial.println(data);
                aliotClient.setTimeout(1);
                led_status = true;
            }
        }
    }
}