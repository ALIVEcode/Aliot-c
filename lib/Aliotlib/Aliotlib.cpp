#include "Aliotlib.hpp"
#include <WiFi.h>

#include <utility>

#define ALIOT_OBJ_ALREADY_SET std::runtime_error("An AliotObj instance already exists and there can only be one AliotObj per program.")
#define NO_ALIOT_OBJ_INSTANCE std::runtime_error("You must create an AliotObj instance before trying to get it.")

//----------------- AliotProject -----------------//


AliotProject::AliotProject(Id project_id)
{
    this->projectId = project_id;
}

void AliotProject::onDocChange(const FieldNames &fieldNames, const ListenerCallback &callback)
{
    auto listener = Listener{callback, fieldNames};
    this->listeners.push_back(listener);
}

void AliotProject::onRecvBroadcast(const ListenerCallback &callback)
{
    auto listener = Listener{callback};
    this->broadcastListeners.push_back(listener);
}

template<typename T>
void AliotProject::updateComponent(Id component_id, T value)
{
}

void AliotProject::broadcast(JsonObject data)
{
}

Id AliotProject::getId() const
{
    return this->projectId;
}

void AliotProject::_handleListen(Fields fields)
{
    for (const auto &listener: this->listeners)
    {
        listener.callback(fields);
    }
}



//----------------- AliotObj -----------------//



AliotObj::AliotObj(Id object_id)
{
    this->objectId = object_id;
}

template<typename T>
void AliotObj::on_action(long action_id, std::function<void(T)> f)
{
}


void AliotObj::add_project(AliotProject project)
{

    this->aliot_projects[project.getId()] = &project;
}

void aliot::connectToWiFi(char *ssid, const char *password, bool log_connection)
{
    WiFi.begin(ssid, password);
    if (log_connection) Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        if (log_connection) Serial.print('.');
        delay(1000);
    }
}


void AliotObj::begin(const char *server_address, const char *path, int port)
{
    WiFiClient wifi;
    if (!this->client)
        this->client = new WebSocketClient(wifi, server_address, port);

    if (!this->client->connected())
        client->begin(path);
}

bool AliotObj::update()
{
    if (client->connected() && client->parseMessage() > 0)
    {
        DynamicJsonDocument response(1024);
        auto err = deserializeJson(response, client->readString());
        if (err)
        {
            // TODO handle errors
        }
        handleOnMessage(response);
    }
    return client->connected();
}

void AliotObj::handleOnMessage(const DynamicJsonDocument &json)
{
    auto event = std::string(json["event"].as<const char *>());
    auto data = json["data"];
    auto project = *this->aliot_projects[data["projectId"]];

    if (event == "action")
    {

    } else if (event == "listen")
    {
        project._handleListen(data["fields"]);
    } else if (event == "broadcast")
    {

    } else if (event == "connected")
    {

    } else if (event == "listener_set")
    {

    } else if (event == "error")
    {

    }
}


void AliotObj::sendEvent(const char *eventName, JsonObject data)
{
}

void AliotObj::updateProjectDoc(const AliotProject &project, Fields fieldsToUpdate)
{
}

JsonObject AliotObj::getProjectDoc(const AliotProject &project)
{
    return {};
}

JsonObject AliotObj::getProjectDocField(const AliotProject &project, Id field)
{
    return {};
}

