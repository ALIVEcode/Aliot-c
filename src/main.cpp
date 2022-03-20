#include <Aliotlib.hpp>
#include <Arduino.h>

#define MAX_LOOP 10

char ssid[] = "helix922.4";
char password[] = "singerose";


AliotObj lumiere = AliotObj("cbd779aa-42d8-4cea-9961-f11abc9cd0f9");
AliotProject projet = AliotProject("be43e75b-f340-4da6-beca-316a60e94e95");


/**
 * @brief DEMO LUMIÈRE
 *
*/
void setup()
{
    delay(10);
    Serial.begin(9600);
    aliot::connectToWiFi(ssid, password);

    projet.onDocChange({"/document/logs"}, [](Fields fields) {
        // auto log_value = fields["/document/logs"];
        Serial.println("aaa");
    });

    lumiere.add_project(projet);
    lumiere.begin();
}


int count = 0;

void loop()
{
    if (lumiere.update() && count++ < MAX_LOOP)
    {
        auto data = JsonObject{};
        data["/document/progress"] = count * 10;
        lumiere.updateProjectDoc(projet, data);

        delay(3000);
    }
}























