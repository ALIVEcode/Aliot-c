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

    if (aliot::connectToAliveCode())
    {
        String data;

        data = String("{\"event\": \"connect_object\", \"data\": {\"id\": \"6560d2db-84e1-4a1e-a68a-2827205f9cd1\"}}");
        webSocketClient.sendData(data);
        Serial.println("Status: ObjectId sent");
    }
    else
    {
        while (1)
            ;
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
                webSocketClient.sendData(data);
                delay(2000);
                Serial.println("Data sent");
                webSocketClient.getData(data);
                while (!(data.length() > 0))
                {
                    webSocketClient.getData(data);
                }
                Serial.println(data);
                aliotClient.setTimeout(1);
                led_status = false;
            }
            else if (!led_status)
            {
                data = String("{\"event\":\"update\",\"data\": {\"projectId\":\"50209e1b-1e1c-4df1-ad9a-3eaffdabed35\",\"fields\":{\"/document/led\": true}}}");
                webSocketClient.sendData(data);
                delay(2000);
                Serial.println("Data sent");
                webSocketClient.getData(data);
                while (!(data.length() > 0))
                {
                    webSocketClient.getData(data);
                }
                Serial.println(data);
                aliotClient.setTimeout(1);
                led_status = true;
            }
        }
    }
}