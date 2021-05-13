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
  serializeJson(classifier_json, LinkSerial);  // send this to server
  delay(1000);
  // Serial.println();
}

int recvFromESP() {
  String json;
  if (linkSerial.available()) {
    StaticJsonDocument<300> c_json;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(c_json, linkSerial);

    while (err == DeserializationError::Ok) {
      if (err == DeserializationError::Ok) {
        serializeJson(c_json, Serial);  // Load from Serial to JsonDocument
        serializeJson(c_json, json);
      } else {
        // Print error to the "debug" serial port
        Serial.print("deserializeJson() returned ");
        //   Serial.println(err.c_str());

        // Flush all bytes in the "link" serial port buffer
        while (linkSerial.available() > 0) linkSerial.read();
      }
    }
  }

  // Check Json
  Serial.println(json);
  if (json == "g") return 1;
  if (json == "p") return 0;
  return 2;  // return error
}
