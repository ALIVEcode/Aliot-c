#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Aliotlib.cpp>
#include <robotBrain.cpp>

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

uint8_t enablePins[] = {EnableFL, EnableFR, EnableBL, EnableBR};
uint8_t directionalPins[] = {MFLA, MFLB, MFRA, MFRB, MBLA, MBLB, MBRA, MBRB};
Robot maVoiture(enablePins, directionalPins);

AliotObj ampoule("e8f599b2-2806-47c2-9c0c-266bb91f89a0");
AliotProject monProject("9cb50433-129e-4905-9144-366c31fade7c");

#define trigPin 27
#define echoPin 36

rangeSensor range1(trigPin, echoPin);

void setup()
{
  systems_initialize();
  range1.init();
  maVoiture.init();
  connectToWiFi();
  ampoule.connect();
}

float distance;

void loop()
{
  if (ampoule.update())
  {
    //*Code du programmeur pour communiquer avec la platforme
  }

  maVoiture.forward(130);
  do
  {
    distance = range1.read();
    Serial.println(distance);

  } while (distance >= 15);

  maVoiture.turnRight(200);
  delay(400);
}