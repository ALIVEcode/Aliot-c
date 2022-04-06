#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>
#include <RobotBrain.cpp>

// Pin assignation
//*Motor DirectionPin front left
#define EnableFL 15 // Enable pin connected to PWM
#define MFLA 22     // Motor Direction
#define MFLB 23     // Directional PIN
//*Motor DirectionPin Front Right
#define EnableFR 4 // Enable pin connected to PWM
#define MFRA 19    // Directional PIN
#define MFRB 21    // Directional PIN
//*Motor DirectionPin Back Left
#define EnableBL 13 // Enable pin connected to PWM
#define MBLA 14     // Directional PIN
#define MBLB 12     // Directional PIN
//*Motor DirectionPin Back Right
#define EnableBR 32 // Enable pin connected to PWM
#define MBRA 25     // Directional PIN
#define MBRB 33     // Directional PIN

#define trigPin 27
#define echoPin 36

//! #define pas obligatoire pour assigner un GPIO
// byte pins[]{MFLA, MFLB, MFRA, MFRB, MBLA, MBLB, MBRA, MBRB}; // Assignation GPIO direction des moteurs
// Voiture maVoiture(pins);                                     // Initialization

uint8_t enablePins[] = {EnableFL, EnableFR, EnableBL, EnableBR};
uint8_t directionalPins[] = {MFLA, MFLB, MFRA, MFRB, MBLA, MBLB, MBRA, MBRB};
Robot maVoiture(enablePins, directionalPins);

rangeSensor range1(trigPin, echoPin);

AliotObj monObjet("c0dd93ff-a17b-46c5-95cc-6a3ebb2f33bf"); // ID de l'objet

#define FORWARD 10
#define BACKWARD 20
#define LEFT 30
#define RIGHT 30

void forward(AliotObjRef obj, JSON data)
{
    maVoiture.forward(data["speed"]);
}

void backward(AliotObjRef obj, JSON data)
{
    maVoiture.backward(data["speed"]);
}

void left(AliotObjRef obj, JSON data)
{
    maVoiture.turnLeft(data["speed"]);
}

void right(AliotObjRef obj, JSON data)
{
    maVoiture.turnRight(data["speed"]);
}
void setup()
{
    systems_initialize();
    range1.init();
    maVoiture.init();
    pinMode(2, OUTPUT);
    bool connected = aliot::connectToWiFi();
    if (!connected)
        END_PROGRAM

    if (!aliot::connectToAliveCode("192.168.0.102", "/"))
        END_PROGRAM

    digitalWrite(2, HIGH);

    monObjet.connect();

    monObjet.registerAction(FORWARD, &forward);
    monObjet.registerAction(BACKWARD, &backward);
    monObjet.registerAction(LEFT, &left);
    monObjet.registerAction(RIGHT, &right);
}

bool ended = false;
float distance;
void loop()
{
    if (!monObjet.update())
    {
        if (!ended)
        {
            ended = true;
            println("Disconnected from ALIVEcode");
        }
        return;
    }

    maVoiture.forward(130);
    do
    {
        distance = range1.read();
        // println(distance);

    } while (distance >= 15);

    maVoiture.turnRight(200);
    delay(400);
}