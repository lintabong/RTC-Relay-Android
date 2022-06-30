#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Stepper.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "KuyKuyKuy"
#define WIFI_PASSWORD "krakatau2"
#define API_KEY "AIzaSyDA78rbEmrJh2xrp3Jg9PsGj9K-xDybGEI"
#define DATABASE_URL "node-controll-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

int r1 = D0;
int r2 = D1;
int r3 = D2;

#define STEPS 32
Stepper stepper(STEPS, D3, D4, D5, D6);

void setup() {
  Serial.begin(115200);
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

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);

  stepper.setSpeed(200);
}

void loop() {
    int door = getdata("/doorlock/");
    int gerb = getdata("/gerbang/");
    int kipa = getdata("/kipas/");

    if (door == 1){
      digitalWrite(r1, HIGH);
    } else if (door == 0){
      digitalWrite(r1, LOW);
    }

    if (gerb == 1){
      digitalWrite(r2, HIGH);
      stepper.step(90);
    } else if (gerb == 0){
      digitalWrite(r2, LOW);
      stepper.step(0);
    }

    if (kipa == 1){
      digitalWrite(r3, HIGH);
    } else if(kipa == 0){
      digitalWrite(r3, LOW);
    }

    Serial.print(door);Serial.print(gerb);Serial.print(kipa);
    Serial.println(" ");
    delay(1000);
}

int getdata(String address){
    int result;
    if (Firebase.RTDB.getInt(&fbdo, address + "/stat")) {
        if (fbdo.dataType() == "int") {
            result = fbdo.intData();
        }
    }
    return result;
}
