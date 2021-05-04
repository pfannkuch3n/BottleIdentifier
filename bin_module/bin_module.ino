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

void loop() {

  // get SRF10 Sensor: address 0x70, 0x72, 0x74
  //i2cScanner();
  int srf1_value, srf2_value, srf3_value;
  srf1_value = getSRF(0x70);
  srf2_value = getSRF(0x72);
  srf3_value = getSRF(0x74);
  //Serial.println(getSRF(0x70));


  // Convert to JSON
  StaticJsonDocument<300> bin_json;
  
  bin_json["hostname"] = "bin";
  JsonArray srf = bin_json.createNestedArray("srf");
  srf.add(srf1_value);
  srf.add(srf2_value);
  srf.add(srf3_value);
  
  // Percentage calcuation 
  char* percentage_value = "100%";


  // Add percentage to json
  bin_json["percentage"] = percentage_value;

  // Send Post request to backend 
  serializeJson(bin_json, Serial); // send this to server
  Serial.println();
  serializeJson(bin_json, linkSerial); 

  

  delay(1000);

}
