#include "srf10.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial linkSerial(19, 18); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps
  linkSerial.begin(4800);
}

bool isEmpty(int value){
  int e_val = 1;
  if ((value <= 13-e_val) || (value >= 13+e_val))
    return 1; // Acceptable, Nothing puting the box
  return 0;
}

void loop() {

  int h100, h50;
  h100 = getSRF(0x70); // h100
  h50 = getSRF(0x72);


  // Convert to JSON
  StaticJsonDocument<300> bin_json;

  bin_json["h100"] = h100;
  bin_json["h50"] = h50;

  
  // Percentage calcuation 
  char* percentage_value = "100%";
  bool s50 = isEmpty(h50);
  bool s100 = isEmpty(h100);

  if (!s100){
    if(!s50){
      percentage_value = "0%";
    }else{
      percentage_value = "50%";
    }
  }else{
    if(!s50){
      percentage_value = "Error";
    }
    else {
      percentage_value = "100%";
    }
  }

  // Add percentage to json
  bin_json["status"] = percentage_value;

  // Send Post request to backend 
  serializeJson(bin_json, Serial); // send this to server
  Serial.println();
  serializeJson(bin_json, linkSerial); 

  delay(1000);

}
