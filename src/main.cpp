#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>
#include <Voiture.cpp>

// // Pin assignation
// #define LED_Status 2
// // #define button 34
// // #define RESET_BUTTON 33
// // #define led 22

// //*Motor DirectionPin front left
// #define EnableFL 15 // Enable pin connected to PWM
// #define MFLA 23     // Motor Direction
// #define MFLB 22     // Directional PIN
// //*Motor DirectionPin Front Right
// #define EnableFR 4 // Enable pin connected to PWM
// #define MFRA 21    // Directional PIN
// #define MFRB 19    // Directional PIN
// //*Motor DirectionPin Back Left
// #define EnableBL 13 // Enable pin connected to PWM
// #define MBLA 12     // Directional PIN
// #define MBLB 14     // Directional PIN
// //*Motor DirectionPin Back Right
// #define EnableBR 32 // Enable pin connected to PWM
// #define MBRA 33     // Directional PIN
// #define MBRB 25     // Directional PIN

// //*Creation de GPIO PWM
// PWM_Pin MEnFL(EnableFL);
// PWM_Pin MEnFR(EnableFR);
// PWM_Pin MEnBL(EnableBL);
// PWM_Pin MEnBR(EnableBR);

//! #define pas obligatoire pour assigner un GPIO
// byte pins[]{MFLA, MFLB, MFRA, MFRB, MBLA, MBLB, MBRA, MBRB}; // Assignation GPIO direction des moteurs
// Voiture maVoiture(pins);                                     // Initialization

AliotObj monObjet("3a92c5b6-bd4f-4dd5-b531-6cfcc04971d7"); // ID de l'objet

void miamiBeach(AliotObj object, JSON data)
{

    DynamicJsonDocument fields (1024);
    fields["/document/pantalon"] = "poire";
    debugPrintJSON(fields);
    object.updateProjectDoc(fields);
    println("lol");
}

void setup()
{
    // pinMode(LED_Status, OUTPUT);
    // maVoiture.init();
    // MEnFL.init();
    // MEnFR.init();
    // MEnBL.init();
    // MEnBR.init();

    // pinMode(led, OUTPUT);
    // pinMode(button, INPUT);
    // pinMode(RESET_BUTTON, INPUT);

    WiFi.mode(WIFI_STA);

    Serial.begin(115200);

    bool connected = aliot::connectToWiFi();
    if (!connected)
        END_PROGRAM

    // digitalWrite(LED_Status, HIGH);

    if (!aliot::connectToAliveCode("10.0.0.45", "/"))
        END_PROGRAM

    monObjet.connect();

    monObjet.registerAction(1, &miamiBeach);
}

bool msg = false;

void loop()
{
    if (!monObjet.update())
    {
        if (!msg)
        {
            msg = true;
            println("Disconnected from ALIVEcode");
        }
        return;
    }
    // MEnFL.analogWrite(170);
    // MEnFR.analogWrite(170);
    // MEnBL.analogWrite(170);
    // MEnBR.analogWrite(170);

    // maVoiture.turnLeft();
    // println("turn Right");

    // delay(2000);

    // maVoiture.turnRight();
    // println("turn left");

    // delay(2000);

    // maVoiture.forward();
    // println("forward");

    // delay(2000);

    // maVoiture.turnRight();
    // println("turn Right");

    // delay(2000);
    // maVoiture.backward();
    // println("Backward");
    //delay(1000);

    // if (!aliot::resetWiFiOnPress(RESET_BUTTON))
    //     END_PROGRAM
}