#include "height.h"
#include "weight.h"
#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial linkSerial(0, 1); // RX, TX

void setup() {
  Wire.begin();                
   Serial.begin(9600);
  // put your setup code here, to run once:
  linkSerial.begin(4800);

  //weightSetup();

//  Serial.begin(57600); delay(10);
  //Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  float calibrationValue = -419.37; // calibration value (see example file "Calibration.ino")
  //calibrationValue = -419.37; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }









  
}

float getBottleHeight(){
 //i2cScanner();
  int h_blue, h_brown;
  //getHeight(&h_blue, &h_brown);
  //Serial.println(*h_top);
  //Serial.println(*h_bottom);
  h_blue = getHCSR();
  //i2cScanner();
  double len_box = 34.0;
  // Serial.println("SRF");
  // Serial.println(h_brown); 
  // Serial.println("HCSR");
  // Serial.println(h_blue/10.0);
   Serial.println(len_box-(h_blue/10.0));
  return len_box - (h_blue/10.0);
}

int inst = 0;
float weight, height, dim;
float forces[5] = {0.00, 0.00, 0.00, 0.00, 0.00};
float positions[5] = {0.00, 0.00, 0.00, 0.00, 0.00};


void loop2() {
  if(Serial.available()>0){
    inst = Serial.parseInt();
  }

  switch(inst){
    case 0:
      weight = 0.0;
      height = 0.0;
      dim = 0.0;
      for(int i=0;i<5;i++){
        forces[i] = 0.0;
        positions[i] = 0.0;
      }
      break;
    case 1:
      weight = getBottleWeight();
      inst = 10;
      break;
    case 2:
      height = getBottleHeight();
      inst = 10;
      break;
    case 3:
      break;
    case 4:
      String str;
      str = "weight: " + String(weight);
      Serial.println(str);
      str = "height: " + String(height);
      Serial.println(str);
      str = "dim: " + String(dim);
      Serial.println(str);
      for(int i=0;i<5;i++){
        str = "Force " + String(i) + " : " + String(forces[i]);
        Serial.println(str);
      }
      Serial.println("Positions");
      for(int i=0;i<5;i++){
        str = "Positions " + String(i) + " : " + String(positions[i]);
        Serial.println(str);
      }
      inst = 10;
      break;
    case 5:
      // to JSON and Send to ESP
      break;
    default:
      Serial.println("No instruciton");
      break;
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
  //serializeJson(classifier_json, Serial); // send this to server
  //Serial.println();

  //Send data to ESP
  // serializeJson(classifier_json, linkSerial); 


}
