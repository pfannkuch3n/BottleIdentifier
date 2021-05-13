#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <SPI.h>

//  MOSI          23
//  MISO          19
//  SCK           18
//  SS(slave)     5

#define _0 0xC0
#define _1 0xFC
#define _RESET 0x00

uint8_t yellow[24] = {_0,_1,_0,_0,_0,_0,_0,_0,_1,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_1,_0,_0};
uint8_t red[24] = {_0,_0,_0,_0,_0,_0,_0,_0,_1,_1,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,_0,_0,_0};
uint8_t green[24] = {_1,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0};


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

  
  SPI.begin();
    // Set frequency to 8MHz
  // Set CPOL and CPHA to 1
  // using MSB format
 
   SPI.beginTransaction (SPISettings (8000000, MSBFIRST, SPI_MODE3));

  while (WiFi.status() != WL_CONNECTED) {

    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void changeLED() {
  if(state == 0){
  for(int i=0;i<24;i++){
  SPI.transfer(red[i]);
  }
  }
  else if(state==1){
  for(int i=0;i<24;i++){
  SPI.transfer(yellow[i]);
  }
  }
  else if(state==2){
  for(int i=0;i<24;i++){
  SPI.transfer(green[i]);
  }
  }
  SPI.transfer(_RESET);
  delay(50);
}


void loop2(){
  if (linkSerial.available()) {
    StaticJsonDocument<300> c_json;
     DeserializationError err = deserializeJson(c_json, linkSerial);
     if (err){
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(err.f_str());
          return;
     }
     serializeJson(c_json, Serial);
     Serial.println();
  }
}
void loop() {
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) {
    Serial.println("Serial availalble");
    StaticJsonDocument<300> c_json;
    DeserializationError err = deserializeJson(c_json, linkSerial);
    serializeJson(c_json, Serial);

    state = c_json["state"];
    
    if (err == DeserializationError::Ok && state == 3 ){
      serializeJson(c_json, json);

      String payload = "";
      StaticJsonDocument<300> tmp;
      tmp["height"] = c_json["height"];
      tmp["weight"] = c_json["weight"];
      tmp["diameter"] = c_json["diameter"];
      tmp["endposition"] = c_json["endposition"];
      tmp["position1"] = c_json["position1"];
      tmp["force"] = c_json["force"];
      serializeJson(tmp,payload);
      // Create connection & send data to server
      HTTPClient http;

      http.begin("http://192.168.1.101:5001/saveclassifier");
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Content-Length", "200");
      http.addHeader("Connection","keep-alive");
      http.addHeader("Accept", "*/*");
      http.addHeader("User-Agent", "Arduino/1.0");
      http.addHeader("Accept-Encoding", "gzip, deflate, br");
      int res = http.POST(payload);
      delay(3000);
      pred_val = http.getString();
      http.end();
      Serial.println(pred_val);
      // return prediction value
      //linkSerial.flush();
      linkSerial.print(pred_val);

      delay(100);
     
    } else {
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
      while (linkSerial.available() > 0) linkSerial.read();
    }
  }

  changeLED();
 
}
