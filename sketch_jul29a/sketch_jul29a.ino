#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "GyverTimer.h"
#include <aREST.h>
#include <aREST_UI.h>

#define WemosD1mini_TX 12 
#define WemosD1mini_RX 13 


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


WiFiServer server(80);
aREST_UI rest = aREST_UI();
float temperature = 0;
float pressure = 0;
float altitude = 0; 


const char* ssid = "Roman_NET";
const char* password = "Kirovakan_1";

String myString; // complete message from arduino, which consistors of snesors data

Adafruit_BMP280 bmp;


SoftwareSerial MySerial(WemosD1mini_RX, WemosD1mini_TX); // RX, TX
String message = "";

GTimer HandleIndexTimer(MS, 1000);
GTimer ClientAvaible(MS, 50);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  rest.variable("temperature",&temperature);
  rest.variable("pressure",&pressure);
  rest.variable("altitude", &altitude);

  rest.set_id("1");
  rest.set_name("esp8266");

  rest.title("ESP8266 UI");
  rest.button(4);
  rest.label("temperature");
  rest.label("pressure");
  rest.label("altitude");


   
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address                                                          
  Serial.println(WiFi.localIP());


  server.begin();
  Serial.println("Server started");  


  
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
  temperature = doc["temperature"];
  pressure = doc["pressure"];
  altitude = doc["altitude"];
  display.display();
}






void loop() {
  
  
 
  if(HandleIndexTimer.isReady()){
      handleIndex();
    }

  WiFiClient client = server.available();
  if (!client) {
      return;
  }
  while(!client.available()){

      if(ClientAvaible.isReady()){
        Serial.println("Waiting for client to be avaible...");
      }
     
  }
  rest.handle(client);


 
 
}
