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

#define FREQUENCY 5000
#define RES 8

// TODO change those macros to send the print command to a virtual terminal (future IoT component)
#define print(x) Serial.print(x)
#define println(x) Serial.println(x)

// events
#define EVT_CONNECT_OBJ "connect_object"
#define EVT_UPDATE "update"
#define EVT_PONG "pong"

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
                println("Connected to ALIVEcode");
            }
            else if (event == "error")
            {
                // TODO handle errors
                print("Errror received '");
                print(event);
                print("': ");
                println(response["data"].as<const char *>());
                continue;
            }

            if (!readyToGo)
                continue;

            // TODO handle callbacks
        }
        return aliotClient.connected();
    }
};

struct Pin
{
    int number;
    int channel;

    Pin(int number, int channel = 0, uint8_t mode = OUTPUT)
    {
        this->number = number;
        this->channel = channel;
        ledcSetup(this->channel, FREQUENCY, RES);
        ledcAttachPin(number, this->channel);
        pinMode(number, mode);
    }

    void write(int value)
    {
        ledcWrite(this->channel, value);
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
    bool connectToWiFi(const char *wifiName = "AliotWiFi", const char *wifiPassword = "password")
    {
        bool connected;
        connected = wm.autoConnect(wifiName, wifiPassword);
        if (!connected)
        {
            println("Connection to WiFi failed");
        }
        else
        {
            println("Connected to WiFi");
            println(WiFi.localIP());
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
        if (!connected)
        {
            println("Connection to ALIVEcode failed");
        }
        return connected;
    }

    bool resetWiFiOnPress(uint8_t button, uint8_t activatedState = LOW)
    {
        if (digitalRead(button) == activatedState)
        {
            wm.resetSettings();
            return aliot::connectToWiFi();
        }
        return true;
    }
};

#endif