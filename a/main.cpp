#include <Arduino.h>
#include <WiFi.h>

char ssid[] = "Helix922.4";
char password[] = "singerose";

// AliotObj lumiere = AliotObj("cbd779aa-42d8-4cea-9961-f11abc9cd0f9");
// AliotProject projet = AliotProject("be43e75b-f340-4da6-beca-316a60e94e95");

//
// void callback(Fields fields)
//{
//    serializeJson(fields, Serial);
//}

/**
 * @brief DEMO LUMIÈRE
 **/
void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    int status = WL_IDLE_STATUS;
    Serial.println("Connecting to WiFi ..");
    while (status != WL_CONNECTED)
    {
        delay(1000);
        status = WiFi.begin(ssid, password);
        Serial.print(".");
    }

    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());

    // projet.onDocChange({"/document/logs"}, callback);

    //    lumiere.addProject(projet);
    //    lumiere.begin((char *) "alivecode.ca", (char *) "/iotgateway/");
}

void loop()
{
    Serial.println("Connected");
    //    while (lumiere.update())
    //    {
    //        DynamicJsonDocument data(1024);
    //        data["/document/progress"] = 10;
    //        lumiere.updateProjectDoc(projet, data);
    //        delay(3000);
    //    }
    Serial.println("Disconnected");
}
