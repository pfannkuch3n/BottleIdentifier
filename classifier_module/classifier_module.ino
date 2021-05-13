#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <math.h>

#include "hardness.h"
#include "height.h"
#include "weight.h"

SoftwareSerial linkSerial(10, 11);  // RX, TX
int state=0;
int inst = 0;
float weight, height;
bool flag = true;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  linkSerial.begin(4800);

  pinMode(A2, INPUT);        // FSR sensor to detect if the lid is closed or not
  pinMode(7, INPUT_PULLUP);  // Button to start the measurement if lid is closed

  weightSetup();
  hardness_setup();
  attachInterrupt(digitalPinToInterrupt(2), readEncoderA, RISING);

  Serial.println("All Setup Complete");
}

float getBottleHeight() {
  int h_blue, h_brown;
  h_blue = getHCSR();
  double len_box = 34.0;
  return len_box - (h_blue / 10.0);
}

void send2ESP() {
  // Convert to JSON
  //  classifier_json["hostname"] = "classifer";
  StaticJsonDocument<300> classifier_json;
  classifier_json["state"] = state;
  classifier_json["weight"] = weight;
  classifier_json["height"] = height;
  classifier_json["diameter"] = diameter;
  classifier_json["force"] = force;
  classifier_json["endposition"] = endposition;
  classifier_json["position1"] = position1;
//  Serial.print(height);
//  Serial.print(",");
//  Serial.print(weight);
//  Serial.print(",");
//  Serial.print(diameter);
//  Serial.print(",");
//  Serial.print(force);
//  Serial.print(",");
//  Serial.print(endposition);
//  Serial.print(",");
//  Serial.print(position1);
//  Serial.print(",");
//  Serial.println("glas");
  // Send Post request to backend
  serializeJson(classifier_json, Serial);  // send this to server
  serializeJson(classifier_json, linkSerial);  // send this to server
  Serial.println();
  Serial.println("Send Complete");
//  delay(1000);
}

void recvFromESP() {
  String json = "";
  if (linkSerial.available()) {
      json = linkSerial.readStringUntil('%');
    }

  Serial.println(json);
  linkSerial.flush();
}

void loop() {
  if (analogRead(A2) > 50) {
    Serial.println("Lid is closed");
    state=1;
    send2ESP();
    if (!digitalRead(7)) {
      Serial.println("Measurements are starting");
      if (getBottleWeight() < 5) {
        Serial.println("please place a bottle");
      } else {
        state = 2;
        send2ESP();
        delay(1000);
        Serial.println("Height measurement in progress");
        height = getBottleHeight();
        delay(1000);

        Serial.println("Weight measurement in progress");
        weight = getBottleWeight();
        Serial.println(weight);

        Serial.println("Hardness measurement in progress");
        measuring();

        state = 2;
        send2ESP();
        
        recvFromESP();
        state = 0;
      }
    }
  }else{
    state = 0;
    weight = 0;
    height = 0;
    diameter = 0;
    force = 0;
    position1 = 0;
    endposition = 0;
  }
  send2ESP();
  delay(1000);
}
