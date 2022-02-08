#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "GyverTimer.h"

#define WemosD1mini_TX 12  //
#define WemosD1mini_RX 13 //



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const char* ssid = "Roman_NET";
const char* password = "Kirovakan_1";

String myString; // complete message from arduino, which consistors of snesors data

Adafruit_BMP280 bmp;



SoftwareSerial MySerial(WemosD1mini_RX, WemosD1mini_TX); // RX, TX
String message = "";
double gas = 0, distance = 0;
GTimer HandleIndexTimer(MS, 1000);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
   
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address                                                          
  Serial.println(WiFi.localIP());






  
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(WiFi.localIP());

  
  
  display.display(); 
}


void handleIndex()
{
  display.clearDisplay();
  display.setCursor(0, 10);
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
      display.println(message);
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

  display.display();
}






void loop() {
  
  
  


  if(HandleIndexTimer.isReady()){
      handleIndex();
    }

  
  
}
