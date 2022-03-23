#ifndef ALIOTC_LIB_ALIOTLIB_H_
#define ALIOTC_LIB_ALIOTLIB_H_

#include <ArduinoJson.h>
#include <WebSocketClient.h>
#include <WiFi.h>

#define Id const char *

#define Fields JsonObjectConst
#define FieldNames JsonArray

typedef void ListenerCallback(Fields fields);

struct Listener
{
    Listener(ListenerCallback callback, FieldNames fieldNames)
    {
        this->callback = callback;
        this->fieldNames = fieldNames;
    }

    explicit Listener(ListenerCallback(callback))
    {
        this->callback = callback;
    }

    ~Listener() = default;

    ListenerCallback *callback;
    FieldNames fieldNames{};
};

class AliotProject
{
private:
    Id projectId{};
//    std::vector<Listener> listeners = {};
//    ststd::vector<Listener> listeners = {};
//    std::vector<Listener> broadcastListeners = {};

public:
    explicit AliotProject(Id _projectId);

    Id getId() const;

    //----------------- add -----------------//
//    void onDocChange(const FieldNames &fieldNames, const ListenerCallback &callback);

//    void onRecvBroadcast(const ListenerCallback &callback);

    //----------------- actions -----------------//

    //----------------- document related -----------------//

    //----------------- automatic handling -----------------//
    void _handleListen(Fields fields);
};

class AliotObj
{
private:
    WiFiClient wifi;
    Id objectId;
    AliotProject *aliotProjects;

    WebSocketClient client;

    void handleOnMessage(const DynamicJsonDocument &json);

    void sendEvent(const char *eventName, const StaticJsonDocument<1024> &data);

public:

    explicit AliotObj(Id _objectId);

    //void addProject(AliotProject project);

    // Websocket
    // TODO add URL in begin
    void begin(char *serverAddress, char *path, int port = 8881);

    bool update();
    // void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    void updateProjectDoc(const AliotProject &project, const StaticJsonDocument<1024> &fieldsToUpdate);
};


inline void connectToWiFi(char *ssid, const char *password)
{
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi ..");
    while (WiFiClass::status() != WL_CONNECTED)
    {
        delay(1000);
    }

    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
}


#endif // ALIOTC_LIB_ALIOTLIB_H_
