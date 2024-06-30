#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define BUZZER 2

const char* ssid = "SSID";
const char* password = "PASSWORD";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
unsigned long lastTime_buzzer = 0;
unsigned long timerDelay_buzzer = 1000;

String serverName = "http://0.0.0.0:5000";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void print(String text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(text);
  display.display(); 
}

void setup(){

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }

    delay(2000);
    display.clearDisplay();

    print("waiting...");

    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    pinMode(BUZZER,OUTPUT);
}

void loop() {
  if ((millis() - lastTime_buzzer) > timerDelay_buzzer) {
    digitalWrite(BUZZER,LOW);
  }
  
  if ((millis() - lastTime) > timerDelay) {
    
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "";
      
      http.begin(serverPath.c_str());

      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        if (payload == "0") {
          Serial.println("pass");
        } else {
          digitalWrite(BUZZER,HIGH);
          lastTime_buzzer = millis(); 
          print(payload);
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
