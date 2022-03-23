#ifndef ALIOT_LIB_H
#define ALIOT_LIB_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

WebSocketClient webSocketClient;
WiFiClient aliotClient;

namespace aliot
{
    /**
     * @brief Tries to connect to the internet. Returns false in case of failure and true in case of success
     *
     * @param wifiName
     * @param wifiPassword
     * @return `true` if the esp32 is connected to the internet and `false` if the esp32 did not connect to the internet
     */
    bool connectToWiFi(const char *wifiName = "AliotWiFi", const char *wifiPassword = "1234")
    {
        WiFiManager wm;
        bool connected;
        connected = wm.autoConnect(wifiName, wifiPassword);
        if (!connected)
        {
            Serial.println("Connection failed");
        }
        else
        {
            Serial.println("Connected");
            Serial.println(WiFi.localIP());
        }
        return connected;
    }

    /**
     * @brief Does a virtual handshake with the alivecode server
     *
     * @return `true` if alivecode likes you and `false` if you should be doing something else with your time!
     */
    bool connectToAliveCode(char *host = "alivecode.ca", char *path = "/iotgateway/")
    {
        webSocketClient.path = path;
        webSocketClient.host = host;
        bool connected = webSocketClient.handshake(aliotClient);
        if (connected)
        {
            Serial.println("Connection successful");
        }
        else
        {
            Serial.println("Connection failed");
        }
        return connected;
    }
};

#endif