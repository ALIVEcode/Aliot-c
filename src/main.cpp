#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>
#include <Voiture.cpp>

// Pin assignation
#define LED_Status 2
// #define button 34
// #define RESET_BUTTON 33
// #define led 22

//*Motor DirectionPin front left
#define EnableFL 15 // Enable pin connected to PWM
#define MFLA 23     // Motor Direction
#define MFLB 22     // Directional PIN
//*Motor DirectionPin Front Right
#define EnableFR 4 // Enable pin connected to PWM
#define MFRA 21    // Directional PIN
#define MFRB 19    // Directional PIN
//*Motor DirectionPin Back Left
#define EnableBL 13 // Enable pin connected to PWM
#define MBLA 12     // Directional PIN
#define MBLB 14     // Directional PIN
//*Motor DirectionPin Back Right
#define EnableBR 32 // Enable pin connected to PWM
#define MBRA 33     // Directional PIN
#define MBRB 25     // Directional PIN

//*Creation de GPIO PWM
PWM_Pin MEnFL(EnableFL);
PWM_Pin MEnFR(EnableFR);
PWM_Pin MEnBL(EnableBL);
PWM_Pin MEnBR(EnableBR);

//! #define pas obligatoire pour assigner un GPIO
byte pins[]{MFLA, MFLB, MFRA, MFRB, MBLA, MBLB, MBRA, MBRB}; // Assignation GPIO direction des moteurs
Voiture maVoiture(pins);                                     // Initialization

AliotObj ampoule("e8f599b2-2806-47c2-9c0c-266bb91f89a0");        // ID de l'objet
AliotProject monProject("9cb50433-129e-4905-9144-366c31fade7c"); // ID du projet

void setup()
{
    pinMode(LED_Status, OUTPUT);
    maVoiture.init();
    // pinMode(led, OUTPUT);
    // pinMode(button, INPUT);
    // pinMode(RESET_BUTTON, INPUT);

    WiFi.mode(WIFI_STA);

    Serial.begin(115200);

    bool connected = aliot::connectToWiFi();
    if (!connected)
        END_PROGRAM

    digitalWrite(LED_Status, HIGH);

    if (!aliot::connectToAliveCode())
        END_PROGRAM

    ampoule.connect();
}

bool msg = false;

void loop()
{
    if (!ampoule.update())
    {
        if (!msg)
        {
            msg = true;
            println("Disconnected from ALIVEcode");
        }
        return;
    }
    MEnFL.analogWrite(170);
    MEnFR.analogWrite(170);
    MEnBL.analogWrite(170);
    MEnBR.analogWrite(170);

    maVoiture.turnLeft();
    println("turn Right");

    delay(2000);

    maVoiture.turnRight();
    println("turn left");

    delay(2000);

    maVoiture.forward();
    println("turn left");

    delay(2000);

    maVoiture.turnRight();
    println("turn left");

    delay(2000);

    maVoiture.backward();
    println("turn left");

    //     JSON fields;
    //     fields["/document/LED"] = false;
    //     fields["/document/Lumiere"] = "red";
    //     monProject.updateDoc(fields);

    // if (!aliot::resetWiFiOnPress(RESET_BUTTON))
    //     END_PROGRAM
}