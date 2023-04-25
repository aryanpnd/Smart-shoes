#define TRIGGER_PIN D6
#define ECHO_PIN D5

#define TRIGGER_PIN2 D7
#define ECHO_PIN2 D8

#define MOTOR1 D1
#define MOTOR2 D2


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
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
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
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
}

void loop() {

  if (Firebase.ready() && signupOK) {

    // let this part as commented for less delay ---------------------------------------------

    int mtr1, mtr2, fs=1, bs=1, fsmin, bsmin, fsmx, bsmx;

    if (Firebase.RTDB.getInt(&fbdo, "variables/Motor1/val")) {
      if (fbdo.dataType() == "int") {
         mtr1 = fbdo.intData();
      }
    }
    if (Firebase.RTDB.getInt(&fbdo, "variables/Motor2/val")) {
      if (fbdo.dataType() == "int") {
         mtr2 = fbdo.intData();
      }
    }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/bsensor/val")) {
    //   if (fbdo.dataType() == "int") {
    //      bs = fbdo.intData();
    //   }
    // }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/fsensor/val")) {
    //   if (fbdo.dataType() == "int") {
    //      fs = fbdo.intData();
    //   }
    // }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/bs1/val")) {
    //   if (fbdo.dataType() == "int") {
    //      bsmx = fbdo.intData();
    //   }
    // }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/bs2/val")) {
    //   if (fbdo.dataType() == "int") {
    //      bsmin = fbdo.intData();
    //   }
    // }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/fs1/val")) {
    //   if (fbdo.dataType() == "int") {
    //      fsmx = fbdo.intData();
    //   }
    // }
    // if (Firebase.RTDB.getInt(&fbdo, "variables/fs2/val")) {
    //   if (fbdo.dataType() == "int") {
    //      fsmin = fbdo.intData();
    //   }
    // }


    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, fs);
    float distance = duration * 0.034 / 2;


    digitalWrite(TRIGGER_PIN2, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN2, LOW);
    long duration2 = pulseIn(ECHO_PIN2, bs);
    float distance2 = duration2 * 0.034 / 2;

    digitalWrite(MOTOR1, mtr1);
    digitalWrite(MOTOR2, mtr2);

    if (Firebase.RTDB.setFloat(&fbdo, "ultrasonic/distance", distance)) {

      //      Serial.println("PASSED");
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");

    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "ultrasonic2/distance", distance2)) {

      //      Serial.println("PASSED");
      Serial.print("Distance: ");
      Serial.print(distance2);
      Serial.println(" cm");

    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    
  } else {
  }

  Serial.println("______________________________");
}