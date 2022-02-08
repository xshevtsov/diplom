
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "GyverTimer.h"

#define WemosD1mini_TX 12  //Mini is D1
#define WemosD1mini_RX 13 //Mini is D2

String message = "";
double gas = 0, distance = 0;

SoftwareSerial MySerial(WemosD1mini_RX, WemosD1mini_TX); // RX, TX

GTimer HandleIndexTimer(MS, 1000);


void setup() {
  Serial.begin(9600);
  MySerial.begin(115200);           

}

void loop() {

  if(HandleIndexTimer.isReady()){
    handleIndex();
  }

}

void handleIndex()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(1024);
 
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc,MySerial);
  // Reading the response
  boolean messageReady = false;
 
  while(messageReady == false) { // blocking but that's ok
    if(MySerial.available()) {
      message = MySerial.readString();
      Serial.println(message);
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  distance = doc["distance"];
  gas = doc["gas"];
  
}
