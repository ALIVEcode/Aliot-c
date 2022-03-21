#include "Aliotlib.hpp"
#include <WiFi.h>

//----------------- AliotProject -----------------//

AliotProject::AliotProject(Id _projectId)
{
    this->projectId = _projectId;
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
void AliotProject::updateComponent(Id componentId, T value)
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



AliotObj::AliotObj(Id _objectId)
{
    this->objectId = _objectId;
}

template<typename T>
void AliotObj::onAction(long actionId, std::function<void(T)> f)
{
}


void AliotObj::addProject(const AliotProject &project)
{
    this->aliotProjects.push_back(project);
}


void AliotObj::begin(const char *serverAddress, const char *path, int port)
{
    this->client = new WebSocketClient(wifi, serverAddress);
    client->begin();
}

bool AliotObj::update()
{
    bool connected = client->connected();
    if (connected && client->parseMessage())
    {
        DynamicJsonDocument response(1024);
        auto err = deserializeJson(response, client->readString());
        if (err)
        {
            // TODO handle errors
        }
        handleOnMessage(response);
    }
    return connected;
}

void AliotObj::handleOnMessage(const DynamicJsonDocument &json)
{
    auto event = std::string(json["event"].as<const char *>());
    auto data = json["data"];
    Id projectId = data["projectId"];

    AliotProject *project;
    for (auto _project: aliotProjects)
    {
        if (std::string(_project.getId()) == projectId)
        {
            project = &_project;
            break;
        }
    }
    if (!project)
    {
        Serial.print("Error, project with id: '");
        Serial.print(projectId);
        Serial.print("' was not added to aliot object with id: '");
        Serial.print(objectId);
        Serial.println("'.");
        return;
    }

    if (event == "action")
    {

    } else if (event == "listen")
    {
        project->_handleListen(data["fields"]);
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


void AliotObj::sendEvent(const char *eventName, const DynamicJsonDocument &data)
{
    DynamicJsonDocument response(1024);
    response["event"] = eventName;
    response["data"] = data;
    serializeJson(response, wifi);
}

void AliotObj::updateProjectDoc(const AliotProject &project, const DynamicJsonDocument &fieldsToUpdate)
{
    sendEvent(project.getId(), fieldsToUpdate);
}

StaticJsonDocument<1024> AliotObj::getProjectDoc(const AliotProject &project)
{
    return {};
}

StaticJsonDocument<512> AliotObj::getProjectDocField(const AliotProject &project, Id field)
{
    return {};
}


//----------------- aliot functions -----------------//

void aliot::connectToWiFi(char *ssid, const char *password)
{
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFiClass::status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());

}



