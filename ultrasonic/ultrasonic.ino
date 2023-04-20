

#define TRIGGER_PIN D6
#define ECHO_PIN D5

#define MOTOR1P1 D1
#define MOTOR1P2 D2

#define MOTOR2P1 D7
#define MOTOR2P2 D8

#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "LE BHIKHARI USE KAR LE"
#define WIFI_PASSWORD "MeowMeow:)"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCZsw5OwhxIES0-9llukkuiUq7KvoKGg74"

// Insert RTDB URL
#define DATABASE_URL "smartshoes-af8ae-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//unsigned long sendDataPrevMillis = 0;
//int count = 0;
bool signupOK = false;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  pinMode(MOTOR1P1, OUTPUT);
  pinMode(MOTOR1P2, OUTPUT);
  pinMode(MOTOR2P1, OUTPUT);
  pinMode(MOTOR2P2, OUTPUT);
}

void loop() {

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

digitalWrite(MOTOR1P1, LOW);
        digitalWrite(MOTOR2P1, LOW);  
  if (Firebase.ready() && signupOK) {

    if (distance < 30) {

      if (Firebase.RTDB.setFloat(&fbdo, "ultrasonic/distance", distance)) {

        //      Serial.println("PASSED");
        digitalWrite(MOTOR1P1, HIGH);
        digitalWrite(MOTOR2P1, HIGH);
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }    
  }
  else{
        digitalWrite(MOTOR1P1, LOW);
        digitalWrite(MOTOR2P1, LOW);
  }
  digitalWrite(MOTOR1P1, LOW);
        digitalWrite(MOTOR2P1, LOW);
  Serial.println("______________________________");
}