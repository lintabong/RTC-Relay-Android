#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#include <virtuabotixRTC.h> 
// clk dat rst
virtuabotixRTC myRTC(D7, D6, D5);

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
int r4 = D3;
int r5 = D4;

int s1 = 0;
int s2 = 0;
int s3 = 0;
int s4 = 0;
int s5 = 0;

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
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
}

void loop() {
    int daon = getdata("/dapur/on");
    int daof = getdata("/dapur/off");
    int k1on = getdata("/kamar_satu/on");
    int k1of = getdata("/kamar_satu/off");
    int k2on = getdata("/kamar_dua/on");
    int k2of = getdata("/kamar_dua/off");
    int rton = getdata("/ruang_tamu/on");
    int rtof = getdata("/ruang_tamu/off");
    int teon = getdata("/teras/on");
    int teof = getdata("/teras/off");

    myRTC.updateTime();
    int rtcH = myRTC.hours;
    int rtcM = myRTC.minutes;
    int RTCc = rtcM + rtcH * 60; 

    if (RTCc <= daof || RTCc >= daon ){
      s1 = 1;
      digitalWrite(r1, HIGH);
    } else{
      s1 = 0;
      digitalWrite(r1, LOW);
    }

    if (RTCc <= k1of || RTCc >= k1on ){
      s2 = 1;
      digitalWrite(r2, HIGH);
    } else{
      s2 = 0;
      digitalWrite(r2, LOW);
    }

    if (RTCc <= k2of || RTCc >= k2on ){
      s3 = 1;
      digitalWrite(r3, HIGH);
    } else{
      s3 = 0;
      digitalWrite(r3, LOW);
    }

    if (RTCc <= rtof || RTCc >= rton ){
      s4 = 1;
      digitalWrite(r4, HIGH);
    } else{
      s4 = 0;
      digitalWrite(r4, LOW);
    }

    if (RTCc <= teof || RTCc >= teon ){
      s5 = 1;
      digitalWrite(r5, HIGH);
    } else{
      s5 = 0;
      digitalWrite(r5, LOW);
    }

    Serial.print(daon);Serial.print(" ");Serial.print(daof);Serial.print(" ");
    Serial.print(k1on);Serial.print(" ");Serial.print(k1of);Serial.print(" ");
    Serial.print(k2on);Serial.print(" ");Serial.print(k2of);Serial.print(" ");
    Serial.print(rton);Serial.print(" ");Serial.print(rtof);Serial.print(" ");
    Serial.print(teon);Serial.print(" ");Serial.print(teof);Serial.print("\t");
    Serial.print(rtcH);Serial.print(":");Serial.print(rtcM);Serial.print("\t");
    Serial.println(RTCc);
    Serial.print(s1);Serial.print(s2);Serial.print(s3);Serial.print(s4);Serial.print(s5);
    Serial.println(" ");
    Serial.println(" "); 
}

int getdata(String address){
    int result;
    int h;
    int m;
    if (Firebase.RTDB.getInt(&fbdo, address + "/jam")) {
        if (fbdo.dataType() == "int") {
            h = fbdo.intData();
        }
    }
    if (Firebase.RTDB.getInt(&fbdo, address + "/menit")) {
        if (fbdo.dataType() == "int") {
            m = fbdo.intData();
        }
    }
    result = m + h * 60;
    
    return result;
}
