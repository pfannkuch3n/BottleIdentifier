#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFi.h>

SoftwareSerial linkSerial(3, 1);  // RX, TX

const char* ssid = "esserver";

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

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

  if (linkSerial.available()) {
    StaticJsonDocument<300> bin_json;
    DeserializationError err = deserializeJson(bin_json, linkSerial); // get data from linkSerial

    if (err == DeserializationError::Ok) {
      serializeJson(bin_json, Serial);  // print to Serial
      serializeJson(bin_json, json);
      Serial.println();
      HTTPClient http;

      // Send request
      http.begin("http://192.168.1.101:5001/savebin");
      http.addHeader("Content-Type", "application/json");
      int res = http.POST(json);

      Serial.print(http.getString());
      http.end();
      delay(1000);

    } else {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
      while (linkSerial.available() > 0) linkSerial.read();
    }
  }
}
