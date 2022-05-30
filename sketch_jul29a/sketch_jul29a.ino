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
#include <AsyncStream.h>


#define WemosD1mini_TX 12 
#define WemosD1mini_RX 13 


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


WiFiServer server(80);
aREST_UI rest = aREST_UI();
float temperature;
float pressure;
float altitude; 

const char* ssid = "Pixel_5986";
const char* password = "0451qwerty";


Adafruit_BMP280 bmp;

SoftwareSerial MySerial(WemosD1mini_RX, WemosD1mini_TX); // RX, TX
AsyncStream<1024> serial(&MySerial, '\n');







GTimer HandleIndexTimer(MS, 5000);
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
 
  rest.label("temperature");
  rest.label("altitude");
  rest.label("pressure");


   
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



void handleClient(){
  WiFiClient client = server.available();
  if (!client) {
      return;
  }
  while(!client.available()){
    delay(10);
//     if(ClientAvaible.isReady()){
//        Serial.println("Waiting for client to be avaible...");
//      }
  }
  rest.handle(client);
  
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
  
 
}





void loop() {

  handleClient();


 
  if(HandleIndexTimer.isReady()){
      handleIndex();
    }

 
  
  if(serial.available()) {
    
    String message = serial.buf;
    Serial.println(message);
    display.println(message);
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
//    temperature = doc["temperature"];
//    pressure = doc["pressure"];
//    altitude = doc["altitude"];
    
    serializeJson(doc, Serial);

    
    display.display();
    
  }
  
  

  handleClient();

 
 
}
