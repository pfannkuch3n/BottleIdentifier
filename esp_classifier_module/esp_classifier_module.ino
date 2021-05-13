#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#define RXD2 16
#define TXD2 17
// Declare the "link" serial port
// Please see SoftwareSerial library for detail
//SoftwareSerial linkSerial(3, 1);  // RX, TX
SoftwareSerial linkSerial(16,17);  // RX, TX

const char* ssid = "esserver";
String  pred_val = "";
// WiFiServer server(80);
int x = 0;
void sendPred(){
  //if (linkSerial.available()) {
    Serial.println("Send pred");
    StaticJsonDocument<300> c_json;
    c_json["pred"] = x;
    //serializeJson(c_json, Serial);
    //serializeJson(c_json, linkSerial);
    if(x>1) x = 0;
    linkSerial.write(x);
      Serial.println(x);
    x++;
  //}

    Serial.println("Send done");
}


void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);

  WiFi.begin(ssid, NULL);
  //  WiFi.begin(ssid,  );

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  // server.begin();
}

void recvFromClassifier() { 
//  Serial.println("Start loop esp");
  String json;

  // Check if the other Arduino is transmitting
  if (linkSerial.available()) {
    Serial.println("Serial availalble");
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the
    // strings
    StaticJsonDocument<300> c_json;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(c_json, linkSerial);

    if (err == DeserializationError::Ok) {
      serializeJson(c_json, Serial);  // send this to server
      serializeJson(c_json, json);




    HTTPClient http;

    // Send request
    http.begin("http://192.168.1.101:5001/saveclassifier");
    http.addHeader("Content-Type", "application/json");
    int res = http.POST(json);
    pred_val = http.getString();
    http.end();
    // int httpResponseCode =
    // http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // Read response
    // Serial.print(http.getString());
    // Serial.print(res);
    char buff[5];


    pred_val.toCharArray(buff,sizeof(buff));
    // Serial.print("pred Value : ");
    // Serial.print(pred_val);
    // Serial.println();
    // Disconnect


    // linkSerial.read();
    
    //linkSerial.write(buff, 1);
    delay(100);
    //free(c_json);
    c_json.clear();
    //StaticJsonDocument<300> r_json;

//    serializeJson(c_json, "g");
    //linkSerial.read();
    //linkSerial.print("g");








      
    } else {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0) linkSerial.read();
    }

    
  }
}

void loop(){
  Serial.println("start looop");
    //recvFromClassifier();
      sendPred();
      delay(1000);
}
