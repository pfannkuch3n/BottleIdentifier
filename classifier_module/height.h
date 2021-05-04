#include <Wire.h>
#include <hcsr04.h>
#include <ArduinoJson.h>

#define TRIG_PIN 12
#define ECHO_PIN 13

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

int getSRF(int address)
{
  int reading = 0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(address); // transmit to device #112 (0x70)
  // the address specified in the datasheet is 224 (0xE0)
  // but i2c adressing uses the high 7 bits so it's 112
  Wire.write(byte(0x00));      // sets register pointer to the command register (0x00)
  Wire.write(byte(0x51));      // command sensor to measure in "inches" (0x50)
  // use 0x51 for centimeters
  // use 0x52 for ping microseconds
  Wire.endTransmission();      // stop transmitting

  // step 2: wait for readings to happen
  delay(70);                   // datasheet suggests at least 65 milliseconds

  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(address); // transmit to device #112
  Wire.write(byte(0x02));      // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      // stop transmitting

  // step 4: request reading from sensor
  Wire.requestFrom(address, 2);    // request 2 bytes from slave device #112

  // step 5: receive reading from sensor
  if (2 <= Wire.available())   // if two bytes were received
  {
    reading = Wire.read();  // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read(); // receive low byte as lower 8 bits
    //Serial.println("reading address");
    //Serial.println(address);
    //Serial.println(reading);   // print the reading
  }

  delay(250);                  // wait a bit since people have to read the output :)
  return reading;
}

// Arduino I2C Scanner
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using GY-87 module for the target
// Tested on 10 September 2019
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.

void i2cScanner()
{
  byte error, address; //variable for error and I2C address
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // wait 5 seconds for the next I2C scan
}


int getHCSR(){
    StaticJsonDocument<200> value;
    DeserializationError error = deserializeJson(value, hcsr04.ToString());

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // try to get Distance from this json format
    // {"Protocol":"Bifrost","Device":"HCSR04","Driver version":"2.0.0","Properties":{"Distance":2617}}
    // Unit: milimeter
    double longitude = value["Properties"]["Distance"];
    // hcsr04.ToString()


    return 10;
    // convert e
}

void getHeight(int *top, int *bottom){
   top = getHCSR();
   bottom = getSRF(0x74); 
}
