#include "WiFi.h"
 
const char* ssid = "esserver";
const char* password =  "";
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, NULL);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {}
