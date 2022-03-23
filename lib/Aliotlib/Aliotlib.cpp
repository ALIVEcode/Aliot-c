#ifndef ALIOT_LIB_H
#define ALIOT_LIB_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>

// events
#define EVT_CONNECT_OBJ "connect_object"
// #define EVNT_

WebSocketClient aliotWebSocketClient;
WiFiClient aliotClient;
WiFiManager wm;

/**
 * @brief This function should NOT be called manually
 *
 * @param eventName
 */
void _sendEvent(const char *eventName, StaticJsonDocument<1024> data)
{
    StaticJsonDocument<1024> event;
    event["event"] = eventName;
    event["data"] = data;
    String eventSerialized;
    serializeJson(event, eventSerialized);
    aliotWebSocketClient.sendData(eventSerialized);
}

struct AliotObj
{
    const char *objectId;

    void connect()
    {
        StaticJsonDocument<1024> data;
        data["id"] = objectId;
        _sendEvent(EVT_CONNECT_OBJ, data);
        aliotClient.setTimeout(1);
    }
};

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
        bool connected;
        connected = wm.autoConnect(wifiName, wifiPassword);
        if (!connected)
        {
            Serial.println("Connection to WiFi failed");
        }
        else
        {
            Serial.println("Connected to WiFi");
            Serial.println(WiFi.localIP());
        }
        return connected;
    }

    /**
     * @brief Does a virtual handshake with the alivecode server
     *
     * @return `true` if alivecode likes you and `false` if you should be doing something else with your time!
     */
    bool connectToAliveCode(const char *host = "alivecode.ca", const char *path = "/iotgateway/", int port = 8881)
    {
        aliotWebSocketClient.path = (char *)path;
        aliotWebSocketClient.host = (char *)host;
        aliotClient.connect(host, port);
        bool connected = aliotWebSocketClient.handshake(aliotClient);
        if (connected)
        {
            Serial.println("Connected to ALIVEcode");
        }
        else
        {
            Serial.println("Connection to ALIVEcode failed");
        }
        return connected;
    }
};

#endif