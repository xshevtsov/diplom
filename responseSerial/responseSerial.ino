#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define WemosD1mini_TX 5  //Mini is D1
#define WemosD1mini_RX 4 //Mini is D2

String message = "Hello";

SoftwareSerial MySerial(WemosD1mini_RX, WemosD1mini_TX); // RX, TX

bool messageReady = true;

void setup() {
  Serial.begin(9600);
  //pinMode(A0, INPUT);
  MySerial.begin(9600);
}

void loop() {
  // Monitor serial communication
  while(MySerial.available()) {
    message = MySerial.readString();
    Serial.println(message);
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["distance"] = random(1,300);
      doc["gas"] = random(1,500);
      serializeJson(doc,MySerial);
      Serial.println("JSON Serialized MySerial");
    }
    messageReady = false;
  }
}
