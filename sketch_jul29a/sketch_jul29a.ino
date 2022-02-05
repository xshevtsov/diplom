#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const char* ssid = "Roman_NET";
const char* password = "Kirovakan_1";

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors


Adafruit_BMP280 bmp;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
   
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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

void loop() {
  delay(100);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(WiFi.localIP());

  display.display();

  
  if (Serial.available() > 0 ) 
    {
      rdata = Serial.read(); 
      myString = myString+ rdata; 
      Serial.print(rdata);
      if( rdata == '\n')
      {
        myString = "";
  
      }
    }

  
}
