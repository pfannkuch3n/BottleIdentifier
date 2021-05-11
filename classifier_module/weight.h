#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 9; //mcu > HX711 dout pin
const int HX711_sck = 8; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void weightSetup() {
//  Serial.begin(57600); delay(10);
  Serial.println();
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


float getRawWeight() {
  int c = 0;
  float ret = 0.0;
  float prev = 0.0;
  int f =1;
  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity
  while(c<100){
   

  
  // check for new data/start next conversion:

  while (!LoadCell.update()) {
    newDataReady = true;
  }

  // get smoothed value from the dataset:
 
  if (newDataReady) {
    ret = LoadCell.getData();
//    if ( (ret - prev) <= 1) f = 0 ;
//    if (millis() > t + serialPrintInterval) {
//      float i = LoadCell.getData();
//      // Serial.print("Load_cell output val: ");
//      // Serial.println(i);
//      ret = i;
//      newDataReady = 0;
//      t = millis();
//    }
  }
  
  // receive command from serial terminal, send 't' to initiate tare operation:
//  if (Serial.available() > 0) {
//    char inByte = Serial.read();
//    if (inByte == 't') LoadCell.tareNoDelay();
//  }

//   check if last tare operation is complete:
//  if (LoadCell.getTareStatus() == true) {
//    Serial.println("Tare complete");
//  }
  c++;
  }

  
  return ret;
  // delay(1000);

}

float getWeight(){
  float error_rate = 100.0;
  float ret = 0.0;
  float prev_value = 0.0;
  int counter = 0;

  while(counter < 10){
    float tmp = getRawWeight();
    if (abs(prev_value - tmp) <= error_rate){
      counter++;
    }
    else counter = 0;
//    Serial.println(tmp);

  }
  Serial.println(ret);
  return ret;
}

// void loop(){
//   Serial.println(getWeight());
// }


float naiveGetWeight() {
  float ret = 0.0;
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      ret = LoadCell.getData();
//      Serial.print("Load_cell output val: ");
//      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
//  if (Serial.available() > 0) {
//    char inByte = Serial.read();
//    if (inByte == 't') LoadCell.tareNoDelay();
//  }

  // check if last tare operation is complete:
//  if (LoadCell.getTareStatus() == true) {
//    Serial.println("Tare complete");
//  }
  delay(100);
  return ret;
}

float getBottleWeight(){
      int counter = 0;
      float ret =0.0; 
      while(counter < 150){
        ret = naiveGetWeight();
        counter++;
      }
  return ret;
}
