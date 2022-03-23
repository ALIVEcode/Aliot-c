#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
// Pin assignation
#define LED_Status 2
#define button 34
#define led 22

char path[] = "/iotgateway/";
char host[] = "alivecode.ca";

WebSocketClient webSocketClient;
WiFiClient client;

void setup()
{
    pinMode(LED_Status, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    WiFi.mode(WIFI_STA);

    Serial.begin(115200);
    WiFiManager wm;

    wm.resetSettings();
    bool res;
    res = wm.autoConnect("AliveIoT", "12345678");
    if (!res)
    {
        Serial.println("Failed connection");
    }
    else
    {
        Serial.println("Connected");
        Serial.println(WiFi.localIP());
        digitalWrite(LED_Status, HIGH);
    }

    if (client.connect("alivecode.ca", 8881))
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Connection failed.");
        while (1)
        {
            delay(1000);
            Serial.println("waiting");
        }
    }

    webSocketClient.path = path;
    webSocketClient.host = host;
    if (webSocketClient.handshake(client))
    {
        Serial.println("Handshake successful");
        String data;

        data = String("{\"event\": \"connect_object\", \"data\": {\"id\": \"6560d2db-84e1-4a1e-a68a-2827205f9cd1\"}}");
        webSocketClient.sendData(data);
        Serial.println("Status: ObjectId sent");
    }
    else
    {
        Serial.println("Handshake failed.");
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
        if (client.connected())
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
                client.setTimeout(1);
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
                client.setTimeout(1);
                led_status = true;
            }
        }
    }
}