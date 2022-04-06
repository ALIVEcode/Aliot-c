#ifndef ALIOT_LIB_H
#define ALIOT_LIB_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>

#define END_PROGRAM \
    while (1)       \
    {               \
    }

#define JSON StaticJsonDocument<1024>

// events
#define EVT_CONNECT_OBJ "connect_object"
#define EVT_UPDATE "update"
#define EVT_PONG "pong"

WebSocketClient aliotWebSocketClient;
WiFiClient aliotClient;
WiFiManager wm;

//*GPIO
#define LED_Status 2 // Wifi active or not

char *wifiName;
char *wifiPassword;

void systems_initialize(void)
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    pinMode(LED_Status, OUTPUT);
}

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

struct AliotProject
{
    const char *projectId;

    AliotProject(const char *projectId)
    {
        this->projectId = projectId;
    }

    void updateDoc(JSON fields)
    {
        JSON data;
        data["projectId"] = projectId;
        data["fields"] = fields;
        _sendEvent(EVT_UPDATE, data);
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
    bool connectToWiFi(char *wifi, char *Password)
    {
        if (!wm.autoConnect(wifiName, wifiPassword))
        {
            Serial.println("Connection to WiFi failed"); //! Changer que ca reset la connection
            return 0;
        }
        Serial.println("Connected to WiFi");
        Serial.println(WiFi.localIP());
        return 1;
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
        if (!aliotWebSocketClient.handshake(aliotClient))
        {
            Serial.println("Connection to ALIVEcode failed");
            return 0;
        }
        return 1;
    }
};

struct AliotObj
{
    const char *objectId;
    bool readyToGo = false;

    AliotObj(const char *objectId)
    {
        this->objectId = objectId;
    }

    void connect()
    {
        if (!aliot::connectToAliveCode())
            END_PROGRAM
        JSON data;
        data["id"] = objectId;
        _sendEvent(EVT_CONNECT_OBJ, data);
    };

    /**
     * @brief Handles the messages sent by the server and calls the relevent functions
     *
     * @return true
     * @return false
     */
    bool update()
    {
        String _res;
        JSON response;
        while (aliotClient.connected() && aliotWebSocketClient.getData(_res))
        {
#ifdef DEBUGGING
            Serial.print("Received data: ");
            Serial.println(_res);
#endif
            // ping
            if (_res == "")
            {
#ifdef DEBUGGING
                Serial.println("Received ping");
                Serial.println(R"(Sending pong... {"event": "pong"})");
#endif
                aliotWebSocketClient.sendData(R"({"event": "pong"})");
                continue;
            }
            deserializeJson(response, _res);

            String event = String(response["event"].as<const char *>());
            if (event == "connected" && !readyToGo)
            {
                readyToGo = true;
                // TODO register listeners
                Serial.println("Connected to ALIVEcode");
            }
            else if (event == "error")
            {
                // TODO handle errors
                Serial.print("Errror received '");
                Serial.print(event);
                Serial.print("': ");
                Serial.println(response["data"].as<const char *>());
                continue;
            }

            if (!readyToGo)
                continue;

            // TODO handle callbacks
        }
        return aliotClient.connected();
    }
};
#endif

void connectToWiFi(char *wifi = "AliveCode", char *password = "12345678")
{
    wifiName = wifi;
    wifiPassword = password;
    if (!aliot::connectToWiFi(wifiName, wifiPassword))
        END_PROGRAM
    digitalWrite(LED_Status, HIGH);
}

void resetWifi(uint8_t button)
{
    if (digitalRead(button))
    {
        wm.resetSettings();
        aliot::connectToWiFi(wifiName, wifiPassword);
    }
}