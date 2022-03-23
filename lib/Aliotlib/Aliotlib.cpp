#ifndef ALIOT_LIB_H
#define ALIOT_LIB_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>

#define ALIOT_FAIL \
    while (1)      \
    {              \
    }

#define JSON StaticJsonDocument<1024>

// events
#define EVT_CONNECT_OBJ "connect_object"
#define EVT_UPDATE "update"

WebSocketClient aliotWebSocketClient;
WiFiClient aliotClient;
WiFiManager wm;

/**
 * @brief This function should NOT be called manually
 *
 * @param eventName
 */
void _sendEvent(const char *eventName, JSON data)
{
    JSON event;
    event["event"] = eventName;
    event["data"] = data;
    String eventSerialized;
    serializeJson(event, eventSerialized);
    aliotWebSocketClient.sendData(eventSerialized);
}

struct AliotProjet
{
    const char *projectId;

    void updateDoc(JSON fields)
    {
        JSON data;
        data["projectId"] = projectId;
        data["fields"] = fields;
        _sendEvent(EVT_UPDATE, data);
    }
};

struct AliotObj
{
    const char *objectId;

    void connect()
    {
        JSON data;
        data["id"] = objectId;
        _sendEvent(EVT_CONNECT_OBJ, data);
        aliotClient.setTimeout(20);
    }

    inline bool update()
    {
        return aliotClient.connected();
    }

    void _()
    {
        JSON responseJson;
        String response;
        aliotWebSocketClient.getData(response);
        while (!(response.length() > 0))
        {
            aliotWebSocketClient.getData(response);
            deserializeJson(responseJson, response);
        }
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