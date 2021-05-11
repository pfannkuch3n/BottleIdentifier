#include <HTTPClient.h>

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#include <WiFi.h>
 
// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(3, 1); // RX, TX

const char* ssid = "esserver";


//WiFiServer server(80);

void wifiSetup()
{
 

}

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);


   WiFi.begin(ssid, NULL );
//  WiFi.begin(ssid,  );
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  //server.begin();


  
}

void loop() {
  String json;
  
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> bin_json;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(bin_json, linkSerial);

    if (err == DeserializationError::Ok) 
    {
      
      serializeJson(bin_json, Serial); // send this to server
      
      serializeJson(bin_json, json);
      Serial.println();
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0)
        linkSerial.read();
    }
  }

//  Serial.println("in the loop");
//  StaticJsonDocument<200> doc; //Create json document
//        
//    doc["h100"] = 123;
//    doc["h50"] = 123;
//    doc["percentage"] = "10%";
//      
//     JsonArray data = doc.createNestedArray("data");
//     data.add(21);
//     data.add(2);
//    String json;
//    serializeJson(doc, json);
//    Serial.println(json);
//






    HTTPClient http;

    // Send request
    http.begin("http://192.168.1.101:5001/savebin");
    http.addHeader("Content-Type", "application/json");
    int res = http.POST(json);

    //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // Read response
    Serial.print(http.getString());
    //Serial.print(res);

    // Disconnect
    http.end();
    delay(1000);

  
}
