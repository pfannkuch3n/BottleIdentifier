#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <math.h>

#include "hardness.h"
#include "height.h"
#include "weight.h"

SoftwareSerial linkSerial(10, 11);  // RX, TX

int inst = 0;
float weight, height;
bool flag = true;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(10, INPUT);
pinMode(11, OUTPUT);
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
  classifier_json["weight"] = weight;
  classifier_json["height"] = height;
  classifier_json["diameter"] = diameter;
  classifier_json["force"] = force;
  classifier_json["endposition"] = endposition;
  classifier_json["position1"] = position1;

  // Send Post request to backend
  // serializeJson(classifier_json, Serial);  // send this to server
  serializeJson(classifier_json, linkSerial);  // send this to server
  Serial.println("Send Complete");
  delay(1000);
}

void recvFromESP() {
  String json = "";
  if (linkSerial.available()) {
      json = linkSerial.readStringUntil('%');
    }

  Serial.println(json);
  linkSerial.flush();
}


void autoLoop() {
  if (analogRead(A2) > 50) {
    Serial.println("Lid is closed");
    if (!digitalRead(7)) {
      Serial.println("Measurements are starting");
      if (getBottleWeight() < 5) {
        Serial.println("please place a bottle");
      } else {
        Serial.println("Height measurement in progress");
        height = getBottleHeight();
        delay(1000);

        Serial.println("Weight measurement in progress");
        weight = getBottleWeight();
        Serial.println(weight);

        Serial.println("Hardness measurement in progress");
        measuring();
        Serial.print(endposition);
        Serial.print("\t");
        Serial.print(force);
        Serial.print("\t");
        Serial.print(diameter);
        Serial.print("\t");
        Serial.println(position1);
      }
    }
  }
  delay(1000);

  //  StaticJsonDocument<200> classifier_json;
  //  classifier_json["height"] = len_box - h_blue_cm;
  // Convert to JSON

  //
  //  classifier_json["hostname"] = "classifer";
  //  classifier_json["weight"] = abs(*h_top - *h_bottom);
  //  classifier_json["hardness"] = "100.00";
  //  classifier_json["weight"] = "1.00";

  // Send Post request to backend
  // serializeJson(classifier_json, Serial); // send this to server
  // Serial.println();

  // Send data to ESP
  // serializeJson(classifier_json, linkSerial);
}


void loop() {
  //inst = 6;

  if (Serial.available() > 0) {
      Serial.println(inst);
    inst = Serial.parseInt();
  }

        weight = 1.1;
      height = 1.1;
      diameter = 1;
      force = 1;
      position1 = 1;
      endposition = 1;
      inst = 5;
 
      String str;
      str = "weight: " + String(weight);
      Serial.println(str);
      str = "height: " + String(height);
      Serial.println(str);
      str = "dim: " + String(diameter);
      Serial.println(str);
      str = "force: " + String(force);
      Serial.println(str);
      str = "pos1: " + String(position1);
      Serial.println(str);
      str = "endpos: " + String(endposition);
      Serial.println(str);
   
    send2ESP();
    //delay(5000);
    //while(flag){
    recvFromESP();
    //}
    delay(2000);


}


//void loop2(){
//    String pred_val = "g";
//    char buff[5];
//    int len = sizeof(buff);
//    pred_val.toCharArray(buff,sizeof(buff));
//    //Serial.println(pred_val);
//    //Serial.println(len);
////    for(int i=0;i<len;i++){
////      Serial.println(buff[i]);
////    }
//
//  
//    //linkSerial.read();
//    linkSerial.print("g");
//    delay(10);
//
//    
////    char x = linkSerial.read();
//
//    
//    Serial.println("Reading x");
//    Serial.println(linkSerial.read());
//    
//  
//    delay(2000);
//}
//
//void loop3(){
//    if (linkSerial.available())
//    Serial.write(linkSerial.read());
//  if (Serial.available())
//    linkSerial.write(Serial.read());
//}
