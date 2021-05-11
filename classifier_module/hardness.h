
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
int diameter;
int x=400;

int force;

void readEncoderA(){
  int b = digitalRead(3);
  if(b>0){
    pos++;
  }else{pos--;}  
}


void hardness_setup() {  // Setup runs once per reset
// initialize serial communication @ 9600 baud:
Serial.begin(9600);

//Define Motor Controller Pins

pinMode(dir1PinA,OUTPUT);
pinMode(dir2PinA,OUTPUT);
pinMode(speedPinA,OUTPUT);
pinMode(2, INPUT);
pinMode(3, INPUT);
pinMode(A0, INPUT);


attachInterrupt(digitalPinToInterrupt(2), readEncoderA, RISING);


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
 diameter = x-position1-minpos;
 
   
}
