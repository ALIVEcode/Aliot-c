#ifndef ALIOT_LIB_H
#define ALIOT_LIB_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

namespace aliot
{

    bool connectToWiFi(const char *wifiName = "AliotWiFi", const char *wifiPassword = "1234")
    {
        WiFiManager wm;
        bool res;
        res = wm.autoConnect(wifiName, wifiPassword);
        if (!res)
        {
            Serial.println("Connection failed");
        }
        else
        {
            Serial.println("Connected");
        }
        return res;
    }

};

#endif