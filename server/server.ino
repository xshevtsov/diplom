#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "painlessMesh.h"
#include <Wire.h>
#include <OneButton.h>

#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "GyverTimer.h"
#include <Arduino_JSON.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 


#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
uint32_t nodeNumber = 999;
uint32_t connectedNodeId = 0;

String readings;
char reportDataForDisplay[128]; 




#define buzzer D3
#define button_pin D6





const char* ssid = "Roman_NET";
const char* password = "Kirovakan_1";

uint32_t now;

Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;


WiFiServer server(80);


float temperature;
float pressure;
float altitude; 

bool displayType = 0;

GTimer ErrorDotDispayTimer(MS, 500);
GTimer UpdateDisplayData(MS, 100);
GTimer ClientAvaible(MS, 50);

void sendMessage();
String getReadings();

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

String getReadings() {
  JSONVar jsonReadings;
  jsonReadings["node"] = (int)nodeNumber;
  jsonReadings["temp"] = bmp.readTemperature();
  jsonReadings["alt"] = bmp.readAltitude(1013.25);
  jsonReadings["pres"] = bmp.readPressure()/100.0F;
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void sendMessage() {
  String msg = getReadings();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
//  snprintf(reportDataForDisplay, sizeof(reportDataForDisplay), "Received from %u \n", from);
  
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["node"];
  double temp = myObject["temp"];
  double hum = myObject["alt"];
  double pres = myObject["pres"];
  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("Alt: ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" hpa");

  

  
}


void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    connectedNodeId = nodeId;
    snprintf(reportDataForDisplay, sizeof(reportDataForDisplay), "--> startHere: New Connection, nodeId = %u\n", nodeId);
}



void changedConnectionCallback() {
  Serial.printf("Changed connection");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

static void handleClick() {
  Serial.println("Clicked!");
  displayType = !displayType;
  
}

OneButton btn = OneButton(
  button_pin,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);




void setup() {
  Serial.begin(115200);

  

  

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

    // Single Click event attachment
  btn.attachClick(handleClick);
  
  // Double Click event attachment with lambda
  btn.attachDoubleClick([]() {
    Serial.println("Double Pressed!");
  });

  
  
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 10);
  
   if(!bmp.begin(BMP280_ADDRESS_ALT) ) { 
    Serial.println("BMP280 SENSOR ERROR"); 
    while(1); 
  }


  
  if(!bmp.begin(BMP280_ADDRESS_ALT) ) { 
    Serial.println("BMP280 SENSOR ERROR"); 
    while(1);
  }


  

  display.clearDisplay();
  display.setCursor(0, 10);


  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
  
  display.display(); 

  

  
}

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration, unsigned long pause) {

  
  pinMode (_pin, OUTPUT );
  analogWriteFreq(frequency);
  analogWrite(_pin,255);
  uint32_t now = millis();
  while (millis () - now < duration) {
    btn.tick();
  }
  
  analogWrite(_pin,0);
  }
  


void musicTest(){
  delay(1000);
  tone(buzzer, 400, 600, 150);
  tone(buzzer, 400, 600, 150);
  tone(buzzer, 400, 600, 150);

  tone(buzzer, 320, 400, 80);
  tone(buzzer, 475, 150, 80);
  tone(buzzer, 400, 600, 100);

  tone(buzzer, 320, 400, 80);
  tone(buzzer, 475, 150, 80);
  tone(buzzer, 400, 600, 100);
}




void loop() {

  mesh.update();

  btn.tick();
 
  
  
  if(displayType){
    display.clearDisplay();
    display.setCursor(0, 10);

    display.println(reportDataForDisplay);
    if(mesh.isConnected(connectedNodeId)){
      display.println(connectedNodeId);
      tone(buzzer, 400, 150, 0);
      display.print(" is currently ONLINE");
    }
    if(!mesh.isConnected(connectedNodeId)){
      display.println(connectedNodeId);
      tone(buzzer, 400, 500, 0);
      display.print(" is currently OFFLINE");
    }
    display.display(); 

 
  }


  if(!displayType){
    if (UpdateDisplayData.isReady()){
      tone(buzzer, 400, 0, 0);
      pressure = bmp.readPressure();
      temperature = bmp.readTemperature();
      altitude = bmp.readAltitude(1013.25);
    
    
          
      display.clearDisplay();
      display.setCursor(0, 10);
      display.print("Node: ");
      display.println(nodeNumber);
      display.print("Temp: ");
      display.println(temperature);
      display.print("Pressure: ");
      display.println(pressure);
      display.print("Altitude: ");
      display.println(altitude);
      display.display(); 


        
    }

   
  }
  


  

  
}
