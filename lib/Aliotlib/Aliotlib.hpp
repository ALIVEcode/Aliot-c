#ifndef ALIOTC_LIB_ALIOTLIB_H_
#define ALIOTC_LIB_ALIOTLIB_H_

#include <utility>
#include <vector>
#include <map>
#include <functional>
#include <numeric>
#include <string>

#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

typedef const char *Id;

typedef JsonObjectConst Fields;
typedef std::function<void(Fields fields)> ListenerCallback;
typedef std::vector<std::string> FieldNames;

struct Listener
{
    Listener(const ListenerCallback &callback, const FieldNames &fieldNames)
    {
        this->callback = callback;
        this->fieldNames = fieldNames;
    }

    explicit Listener(const ListenerCallback &callback)
    {
        this->callback = callback;
    }

    ~Listener() = default;

    ListenerCallback callback;
    FieldNames fieldNames{};

    std::string getFieldsString()
    {
        return "[" + std::accumulate(fieldNames.begin(), fieldNames.end(), std::string("")) + "]";
    };
};

typedef std::map<int, std::function<void()>> mapProtocols;

class AliotProject
{
private:
    Id projectId{};
    std::vector<Listener> listeners = {};
    std::vector<Listener> broadcastListeners = {};

public:
    explicit AliotProject(Id _projectId);

    Id getId() const;

    //----------------- add -----------------//
    void onDocChange(const FieldNames &fieldNames, const ListenerCallback &callback);

    void onRecvBroadcast(const ListenerCallback &callback);

    //----------------- actions -----------------//

    void broadcast(JsonObject data);

    template<typename T>
    void updateComponent(Id componentId, T value);

    //----------------- document related -----------------//




    //----------------- automatic handling -----------------//
    void _handleListen(Fields fields);
};

class AliotObj
{
private:
    Id objectId{};
    mapProtocols protocols = {};
    bool running = false;
    std::vector<AliotProject> aliotProjects{};

    WiFiClient wifi;
    WebSocketClient *client = nullptr;

    void handleOnMessage(const DynamicJsonDocument &json);

    void sendEvent(const char *eventName, const DynamicJsonDocument &data);

public:

    explicit AliotObj(Id _objectId);

    // Decorators
    template<typename T>
    void onAction(long actionId, std::function<void(T value)> f);

    void addProject(const AliotProject &project);

    // Websocket
    // TODO add URL in begin
    void begin(const char *serverAddress = "wss://alivecode.ca/iotgateway", const char *path = "/", int port = 8881);

    bool update();
    // void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    void updateProjectDoc(const AliotProject &project, const DynamicJsonDocument &fieldsToUpdate);

    StaticJsonDocument<1024> getProjectDoc(const AliotProject &project);

    StaticJsonDocument<512> getProjectDocField(const AliotProject &project, Id field);

};

namespace aliot
{
    void connectToWiFi(char *ssid, const char *password);
}

#endif // ALIOTC_LIB_ALIOTLIB_H_
