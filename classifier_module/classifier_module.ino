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

  weightSetup();
}

void loop() {
  // Get hardness 



  // Get Weight, not accurate value 
  float weight = getWeight();
  //Serial.println(weight);

  
  //Get Height
  //i2cScanner();
  int *h_top, *h_bottom;
  getHeight(h_top, h_bottom);
  //Serial.println(*h_top);
  //Serial.println(*h_bottom);
  //i2cScanner();
  Serial.println("SRF");
  Serial.println(getSRF(0x71)); 
  Serial.println("HCSR");
  Serial.println(getHCSR());
  delay(1000);
    // Convert to JSON
//  StaticJsonDocument<200> classifier_json;
//  
//  classifier_json["hostname"] = "classifer";
//  classifier_json["weight"] = abs(*h_top - *h_bottom);
//  classifier_json["hardness"] = "100.00";
//  classifier_json["weight"] = "1.00";


  // Send Post request to backend 
  //serializeJson(classifier_json, Serial); // send this to server
  Serial.println();

  //Send data to ESP
  // serializeJson(classifier_json, linkSerial); 


}
