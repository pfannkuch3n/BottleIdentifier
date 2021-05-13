#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFi.h>

SoftwareSerial linkSerial(16,17);  // RX, TX

const char* ssid = "esserver";
String  pred_val = "";


void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  linkSerial.begin(4800);

  WiFi.begin(ssid, NULL);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void loop() { 
  String json;

  // Check if the other Arduino is transmitting
  if (linkSerial.available()) {
    Serial.println("Serial availalble");
    StaticJsonDocument<300> c_json;

    DeserializationError err = deserializeJson(c_json, linkSerial);

    if (err == DeserializationError::Ok) {
      serializeJson(c_json, Serial);  // send this to server
      serializeJson(c_json, json);

    // Create connection & send data to server
    HTTPClient http;

    http.begin("http://192.168.1.101:5001/saveclassifier");
    http.addHeader("Content-Type", "application/json");
    int res = http.POST(json);
    
    pred_val = http.getString();
    http.end();

    // return prediction value
    linkSerial.print(pred_val);

    delay(100);

    } else {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
      while (linkSerial.available() > 0) linkSerial.read();
    }
  }
}
