#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFi.h>

SoftwareSerial linkSerial(16, 17);  // RX, TX

const char* ssid = "esserver";
String pred_val = "";
uint8_t state = 0;
String json;

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

void changeLED() {
  // Serial.println(state);
}

void loop() {
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) {
    Serial.println("Serial availalble");
    StaticJsonDocument<500> c_json;
    Serial.println("1");
    DeserializationError err = deserializeJson(c_json, linkSerial);
    Serial.println("2");
    // Serial.println(err);
    serializeJson(c_json, Serial);
    Serial.println("3");
    state = c_json['state'];
    Serial.println("4");

    if (err == DeserializationError::Ok && state == 2) {
      Serial.println("5");
      changeLED();  // Chanage LED for state 2 (Green)
      serializeJson(c_json, Serial);
      Serial.println("6");
      serializeJson(c_json, json);

      // Create connection & send data to server
      HTTPClient http;

      http.begin("http://192.168.1.101:5001/saveclassifier");
      http.addHeader("Content-Type", "application/json");
      int res = http.POST(json);
      delay(3000);
      pred_val = http.getString();
      http.end();
      Serial.println(pred_val);
      // return prediction value
      linkSerial.flush();
      linkSerial.print(pred_val);

      delay(100);
      state = 0;
    } else {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
      while (linkSerial.available() > 0) linkSerial.read();
    }
  }

  changeLED();
}
