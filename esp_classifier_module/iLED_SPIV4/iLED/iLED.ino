// controlling iLED using SPI

// Arduino SPI
// Pins (for Mega1280; https://www.arduino.cc/en/reference/SPI)
// SPI pins   Arduino pin
//  MOSI          23
//  MISO          19
//  SCK           18
//  SS(slave)     5

// iLED hardware info (https://www.ett.co.th/prod2014/ET-iLED/Manual%20of%20ET-iLED-RGB_Eng.pdf)
// SPI frequency = 8MHz
// CPOL and CPHA = 1
// MSB is used
//  Data_code_0 = SPI byte 0xC0
//  Data_code_1 = SPI byte 0xFC
//  Data_reset  = SPI byte 0x00
//  Note: 1 SPI byte is acting as 1 bit for iLED data 

#include <SPI.h>

#define _0 0xC0     // Data_code_0 = SPI byte 0xC0
#define _1 0xFC     // Data_code_1 = SPI byte 0xFC
#define _RESET 0x00 // Data_reset  = SPI byte 0x00

// FC + FC00*10^n



int status1 = 1;
uint32_t lastBounce = 0;
uint32_t lastBounce2 = 0;
int reqDelay = 600;
int reqDelay1 = 200;
bool mood1 = false;

int reqDelay2 = 500;
int reqDelay3 = 50;
int delayBuffer;
int counter2 = 0;
bool twice= true;
int push = 0;
uint32_t push2 = 0;

int j = 0;
int mainStart=0;
int byteLength = 8;
int mainEnd;
uint8_t MainColor;

uint8_t status2 = 0;
uint8_t yellow[24] = {_0,_1,_0,_0,_0,_0,_0,_0,_1,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_1,_0,_0};

uint8_t datas[24];
float green =0;
float red=0;
float blue=0;
int greenVal;
int redVal;
int blueVal; 

float rgb[] = {0,1,0};
float changing[]= {0,1,0};

float fade = 0.01;
int fade2 = 1;

int brightness = 254;
 int lastBrightness;

bool on = true;



void setup() {
  // Initializing SPI

noInterrupts();
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), button1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), button2, FALLING);
 
  Serial.begin(19200);
  SPI.begin();
    // Set frequency to 8MHz
  // Set CPOL and CPHA to 1
  // using MSB format
 
   SPI.beginTransaction (SPISettings (8000000, MSBFIRST, SPI_MODE3));
interrupts();

  Serial.println(fade);

}


void button1(){
if(on){
  if((millis() - lastBounce) > reqDelay){
      Serial.print(millis());
      Serial.print(" ");
    
      Serial.print(lastBounce);
      Serial.println(" ");

      uint16_t counter3 = 0;
      while(!digitalRead(2)){
        counter3++;
        delay(10);
      }
  if(counter3 <1000 && mood1){
  status1++;
  if( status1 > 4){
    status1 = 1;
  }
  }
  else if(counter3 >=1000){
    Serial.println("ON");
    mood1 = !(mood1);
  }
  if(mood1){
    switch(status1){
      case 1: whiteLight(); break;
      case 2: yellowLight(); break;
      case 3: rgbLight(); break;
      case 4: changingRGB(); break;  
    }
  }else{ off();}
  Serial.println(status1);
  Serial.println(counter3);
   
  }
 lastBounce = abs(millis());

}
}
void button2(){
if(mood1){   
  if((millis() - lastBounce2) > reqDelay1){
     //Serial.println(digitalRead(3));
      uint16_t counter3 = 0;
      while(!digitalRead(3)){
        if((counter3 >1000) && (on)){
          Serial.println("long");
          trigger();
        }
        counter3++;
        delay(20);
      }if(counter3 < 1000){
        Serial.println(twice);
        twice = !twice;
        if(!twice){
          push2 = millis();
        }
        else if(twice){
          if((millis() - push2) < 1000){
            Serial.println("double");
            sleepMode();
          }
          }
        }
  }
 lastBounce2 = millis();
}
}

void sleepMode(){
  Serial.println("sleepMode");
  on = !on;

  if(!on){
    lastBrightness = brightness;
      for( int i = lastBrightness; i>=0; i--){
        brightness = i;
        updateData();
        transferData();
        delay(150);
      }
  }else{
    for( int i = 0; i<=lastBrightness; i++){
        brightness = i;
        updateData();
        transferData();
        delay(150);
       }
    }
}
void trigger(){
  if((status1 ==1) || (status1 ==2)){
    brightness += fade2;
    if((brightness >= 255) || (brightness <= 0)){
      fade2 = -fade2;
    }
  updateData();
  transferData();
  Serial.println(brightness);
  delay(500);
  
  }
  else if(status1 ==3){
    mixing(rgb);
    delay(100);
  }
}




void off(){
  Serial.println("off");
  green = 0;
  red=0;
  blue=0;
}

void whiteLight(){
  Serial.println("white");
 green =1;
 red=1;
 blue=1;
}
void yellowLight(){
  Serial.println("yellow");

  green =0.5;
  red=1;
  blue=0;
  
}


void rgbLight(){
  Serial.println("rgb");
  green = rgb[0];
  red = rgb[1];
  blue = rgb[2];
}
  
void changingRGB(){
  Serial.println("changing");
}

void mixing(float color[3]){
  Serial.println("mix");
  if(color[2] <=(0+fade) && color[1] >(0+fade)){
    color[1] -=fade;
    color[0] +=fade;
  }
  else if( color[1] <=(0+fade) && color[0]>(0+fade)){
    color[2] +=fade;
    color[0] -=fade;
  }
  else if (color[0] <=(0+fade)){
    color[2] -=fade;
    color[1] +=fade;
  }
  green = color[0];
  red = color[1];
  blue = color[2];

  if (status1 == 3){
    rgb[0]= color[0];
    rgb[1]= color[1];
    rgb[2]= color[2]; 
  }
  else if (status1 == 4){
    changing[0]= color[0];
    changing[1]= color[1];
    changing[2]= color[2]; 
  }
  updateData();
  transferData();
}


void updateData(){

  greenVal = int(round(green*brightness));
  redVal= int(round(red*brightness));
  blueVal = int(round(blue*brightness));

  for(int i = 0; i<8; i++){
    datas[7-i]= bitRead(greenVal, i)? _1:_0;
  }
  for(int i = 0; i<8; i++){
    datas[15-i]= bitRead(redVal, i)? _1:_0;
  }
  for(int i = 0; i<8; i++){
    datas[23-i]= bitRead(blueVal, i)? _1:_0;
  }

 /* for(int i =0; i<24; i++){
    Serial.print(datas[i], HEX);
  }
  Serial.println("");
*/
delay(50);
}

void transferData(){
  for (int i =0; i<24; i++){
    SPI.transfer(datas[i]);
  }
  SPI.transfer(_RESET);
  delay(50);
}

void loop() {

/*mixing(rgb);
for (int i =0; i<3; i++){
  Serial.print(rgb[i]);
  Serial.print(" ");
}
Serial.println("");
*/
if((status1 ==4) && (mood1)){
  mixing(changing);
}

updateData();
transferData();
//Serial.println(greenVal);


}
