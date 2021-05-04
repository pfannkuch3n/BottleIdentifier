#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#include<WiFi.h>
// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(3, 1); // RX, TX


// WIFI SETUP 
#define SSID_NAME "TP-LINK_DDEC54"
#define SSID_PASS "admin"
IPAddress local_ip = {192, 168, 1, 1};
IPAddress gateway = {192, 168, 1, 1};
IPAddress subnet = {255, 255, 255, 0};

void wifiSetup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(SSID_NAME, SSID_PASS);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  delay(60000);
  WiFi.softAPdisconnect();
  Serial.print("SoftAPdisconnect");
}

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);

  wifiSetup();
}
 
void loop() {
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> classifier_json;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(classifier_json, linkSerial);

    if (err == DeserializationError::Ok) 
    {
      Serial.println("Classifier Ino32");
      serializeJson(classifier_json, Serial); // send this to server
      Serial.println();
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0)
        linkSerial.read();
    }
  }
}



