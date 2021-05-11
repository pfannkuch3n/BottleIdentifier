//Code by Reichenstein7 (thejamerson.com)

//Keyboard Controls:
//
// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
//
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 1
int dir1PinA = 4;
int dir2PinA = 5;
int speedPinA = 6
;// Needs to be a PWM pin to be able to control motor speed

int inputSpeed;
int inputDir;
int higher = 100;

int pos = 0;
int minpos;
int position1;
int endposition;
int durchmesser;
int x=400;

int force;
void setup() {  // Setup runs once per reset
// initialize serial communication @ 9600 baud:
Serial.begin(9600);

//Define L298N Dual H-Bridge Motor Controller Pins

pinMode(dir1PinA,OUTPUT);
pinMode(dir2PinA,OUTPUT);
pinMode(speedPinA,OUTPUT);
pinMode(2, INPUT);
pinMode(3, INPUT);
pinMode(A0, INPUT);

//pinMode(A0, INPUT_PULLUP);
//pinMode(A1, INPUT_PULLUP);

attachInterrupt(digitalPinToInterrupt(2), readEncoderA, RISING);


}

void readEncoderA(){
  int b = digitalRead(3);
  if(b>0){
    pos++;
  }else{pos--;}  
}

void setMotor(int speedSign){

if (speedSign >0){
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
}else if( speedSign <0){
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
}else{
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, LOW);
}

analogWrite(speedPinA, abs(speedSign));//Sets speed variable via PWM 

  
}

void measuring(){
 //Start measuring with pushing forward
 
 setMotor(-30);
 int lastpos = pos;
  delay(100);
  while(lastpos != pos){
    lastpos = pos;
    delay(100);
  }
 minpos = pos;
 setMotor(0);
 delay(500);
 
 setMotor(30);
 delay(1000);
 
 position1 = pos;
 
 setMotor(0);
 delay(500);
 
 setMotor(200);
 delay(300);
 
 endposition = pos-position1;
 force= analogRead(A0);

 setMotor(0);
 delay(200);
 
 setMotor(-30);
 delay(1000);
  
 setMotor(0);

//Calculation of the main values
 durchmesser = x-position1-minpos;
 
   
}

void sendDatas(int endpos,int endforce, int diameter){
  endpos=endposition;
  endforce=force;
  diameter=durchmesser;
}

void loop() {
  measuring();
    Serial.print(endposition);
    Serial.print("\t");
    Serial.print(force);
    Serial.print("\t");
    Serial.print(durchmesser);
    Serial.print("\t");
    Serial.println(position1);

  delay(10000);
}
