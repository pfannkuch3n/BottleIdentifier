#include "height.h"
#include "weight.h"
#include "hardness.h"
#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial linkSerial(0, 1); // RX, TX

void setup() {
  Wire.begin();                
   Serial.begin(9600);
  // put your setup code here, to run once:
  linkSerial.begin(4800);
  pinMode(A2, INPUT); //FSR sensor to detect if the lid is closed or not
  pinMode(7, INPUT_PULLUP); //Button to start the measurement if lid is closed
  weightSetup();

  hardness_setup();
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



void loop() {
  if(analogRead(A2)>50){
    if(!digitalRead(7)){
      if(getRawWeight()<5){
        Serial.println("please place a bottle");
      }else{
      Serial.println("Height measurement in progress");
      height = getBottleHeight();
      delay(1000);

      Serial.println("Weight measurement in progress");
      int counter = 0; 
      while(counter < 150){
        weight = getRawWeight();   
        counter++;
      }
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
  //serializeJson(classifier_json, Serial); // send this to server
  //Serial.println();

  //Send data to ESP
  // serializeJson(classifier_json, linkSerial); 


}
