#include <Arduino.h>
#include "Aliotlib.hpp"

char ssid[] = "Helix922.4";
char password[] = "singerose";


AliotObj lumiere = AliotObj("cbd779aa-42d8-4cea-9961-f11abc9cd0f9");
AliotProject projet = AliotProject("be43e75b-f340-4da6-beca-316a60e94e95");

/**
 * @brief DEMO LUMIÈRE
 **/
void setup()
{
    Serial.begin(9600);
    aliot::connectToWiFi(ssid, password);

    projet.onDocChange({"/document/logs"}, [](Fields fields) {
        serializeJson(fields, Serial);
    });

    lumiere.addProject(projet);
}


void loop()
{
    Serial.println("Connected");
    lumiere.begin();
    while (lumiere.update())
    {
        DynamicJsonDocument data(1024);
        data["/document/progress"] = 10;
        lumiere.updateProjectDoc(projet, data);
        delay(3000);
    }
    Serial.println("Disconnected");
}
