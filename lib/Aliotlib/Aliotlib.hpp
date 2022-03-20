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
    explicit AliotProject(Id project_id);

    Id getId() const;

    //----------------- add -----------------//
    void onDocChange(const FieldNames &fieldNames, const ListenerCallback &callback);

    void onRecvBroadcast(const ListenerCallback &callback);

    //----------------- actions -----------------//

    void broadcast(JsonObject data);

    template<typename T>
    void updateComponent(Id component_id, T value);

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
    std::map<const char *, AliotProject*> aliot_projects{};
    WebSocketClient *client = nullptr;

    void handleOnMessage(const DynamicJsonDocument &json);

    void sendEvent(const char *eventName, JsonObject data);

public:

    explicit AliotObj(Id object_id);

    // Decorators
    template<typename T>
    void on_action(long action_id, std::function<void(T value)> f);

    void add_project(AliotProject project);

    // Websocket
    // TODO add URL in begin
    void begin(const char *server_address = "wss://alivecode.ca", const char *path = "/iotgateway/", int port = 8881);

    bool update();
    // void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    void updateProjectDoc(const AliotProject &project, Fields fieldsToUpdate);

    JsonObject getProjectDoc(const AliotProject &project);

    JsonObject getProjectDocField(const AliotProject &project, Id field);

};

namespace aliot
{
    void connectToWiFi(char *ssid, const char *password, bool log_connection = true);
}

#endif // ALIOTC_LIB_ALIOTLIB_H_
