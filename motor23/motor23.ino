<<<<<<< HEAD:motor23/motor23.ino
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
int prevT =0;
float ePrev = 0;
float eIntegral = 0;

int pos1;
int pos2;
int pos3;
int pos4;
int pos5;
int position[5];

int force2;
int force3;
int force4;
int force5;
int force[5];

int maxpos;
int minpos;


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

void initialize(){

  setMotor(30);
  int lastpos = pos;
  delay(100);
  while(lastpos != pos){
    Serial.print(lastpos);
    Serial.print("\t");
    Serial.println(pos);
    lastpos = pos;
    delay(100);
  }
  maxpos = pos;
  Serial.println(maxpos);
  setMotor(0);
  delay(200);
  Serial.println("Stop");
  setMotor(-30);
  delay(100);
  while(lastpos != pos){
    Serial.print(lastpos);
    Serial.print("\t");
    Serial.println(pos);
    lastpos = pos;
    delay(100);
  }
  minpos = pos;
  setMotor(0);
  Serial.println(minpos);
}

void measuring(){
 //Start measuring with pushing forward

 setMotor(30);
 delay(1000);
 position[0] = pos;
 force[0] = analogRead(A0);
 
      
 setMotor(0);
 delay(500);
 setMotor(60);
 delay(400);
 pos[1] = pos;
 force[1] = analogRead(A0);
 
 setMotor(0);
 delay(500);
 
 setMotor(120);
 delay(400);
 pos[2] = pos;
 force[2] = analogRead(A0);
 
 setMotor(0);
 delay(500);
 
 setMotor(180);
 delay(400);
 pos[3] = pos;
 force[3] = analogRead(A0);
 
 setMotor(0);
 delay(1000);
 
 setMotor(200);
 delay(300);
 pos[4] = pos;
 force[4] = analogRead(A0);

 setMotor(0);
 delay(200);
 
 setMotor(-30);
 delay(100);
  
 setMotor(0);
  
}

void sendDatas(int[] positions, int[] forces){
for( int i=0; i<5; i++){
  positions[i]=position[i];
  forces[i] = force[i];
}

void loop() {


}
