#include "Aliotlib.hpp"

//----------------- AliotProject -----------------//

AliotProject::AliotProject(Id _projectId)
{
    this->projectId = _projectId;
}

//void AliotProject::onDocChange(const FieldNames &fieldNames, const ListenerCallback &callback)
//{
//    auto listener = Listener{callback, fieldNames};
//    this->listeners.push_back(listener);
//}

//void AliotProject::onRecvBroadcast(const ListenerCallback &callback)
//{
//    auto listener = Listener{callback};
//    this->broadcastListeners.push_back(listener);
//}

Id AliotProject::getId() const
{
    return this->projectId;
}

void AliotProject::_handleListen(Fields fields)
{
//    for (const auto &listener: this->listeners)
//    {
//        listener.callback(fields);
//    }
}



//----------------- AliotObj -----------------//



AliotObj::AliotObj(Id _objectId)
{
    this->objectId = _objectId;
}


//void AliotObj::addProject(AliotProject project)
//{
//    this->aliotProjects = &project;
//}


void AliotObj::begin(char *serverAddress, char *path, int port)
{
    client.path = path;
    client.host = serverAddress;
    if (client.handshake(wifi))
    {
        Serial.println("Connection successful");
    } else
    {
        Serial.println("Connection failed");
    }
}

bool AliotObj::update()
{
    bool connected = wifi.connected();
    String dataReceived;
    if (connected && client.getData(dataReceived) && dataReceived.length() != 0)
    {
        DynamicJsonDocument response(1024);
        auto err = deserializeJson(response, dataReceived);
        if (err)
        {
            // TODO handle errors
        }
        serializeJson(response, Serial);
        handleOnMessage(response);
    }
    return connected;
}

void AliotObj::handleOnMessage(const DynamicJsonDocument &json)
{
    auto event = String(json["event"].as<const char *>());
    auto data = json["data"];
    Id projectId = data["projectId"];
    auto project = this->aliotProjects;

    if (String(projectId) != this->aliotProjects->getId())
    {
        project = nullptr;
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


inline void AliotObj::sendEvent(const char *eventName, const StaticJsonDocument<1024> &data)
{
    DynamicJsonDocument response(1024);
    response["event"] = eventName;
    response["data"] = data;
    String dataToSend;
    serializeJson(response, dataToSend);
    client.sendData(dataToSend);
    Serial.println("Sent!");
}

void AliotObj::updateProjectDoc(const AliotProject &project, const StaticJsonDocument<1024> &fieldsToUpdate)
{
    sendEvent(project.getId(), fieldsToUpdate);
}


//----------------- aliot functions -----------------//




