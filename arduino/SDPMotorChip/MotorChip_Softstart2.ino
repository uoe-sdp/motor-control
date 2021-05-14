#include <Wire.h>

unsigned long timemilli = 0;
int TIMECHECKMILLI = 1;

int motorPower[8] = {255, 255, 255, 255, 255, 255,0,0};
int motorEnablePin[8] = {6, 3, 11, 10, 9, 5,0,0};
int motorFPin[8] = {1, 4, A1, A3, 13, 7,0,0};
int motorBPin[8] = {0, 2, A0, A2, 12, 8,0,0};
int motorMode[8] = {0,0,0,0,0,0,0,0};
int actualPower[8] = {255,255,255,255,255,255,255,255};
int wantedPower[8] = {0,0,0,0,0,0,0,0};
int stopAll = 0;

//0=float, 1 = brake, 2 = forwards, 3 = backwards
void setup()
{
   Wire.begin(4);
   Wire.onReceive(receiveEvent);
//   Serial.begin(9600);
//   Serial.println("hi");
   pinMode(0, OUTPUT);
   pinMode(1, OUTPUT);
   pinMode(2, OUTPUT);
   pinMode(3, OUTPUT);
   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
   pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
   pinMode(11, OUTPUT);
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(A0, OUTPUT);
   pinMode(A1, OUTPUT);
   pinMode(A2, OUTPUT);
   pinMode(A3, OUTPUT);
}

void loop()
{
  if (millis() - timemilli > TIMECHECKMILLI){
  timemilli = millis();
  if (stopAll == 1){
    for (int i = 0; i < 6; i++){
         motorMode[i] = 0;
       }
       stopAll = 0;
  }
  for (int i = 0; i < 6; i++){
    switch (motorMode[i]){
      case 0: //float the motor
        digitalWrite(motorEnablePin[i], LOW);
        digitalWrite(motorFPin[i], LOW);
        digitalWrite(motorBPin[i], LOW);
        actualPower[i] = 255;
      break;
      case 1: //brake the motor
        digitalWrite(motorEnablePin[i], HIGH); // make 255 to actually cause braking.. (set to 0 to prevent motor damage)
        digitalWrite(motorFPin[i], LOW);
        digitalWrite(motorBPin[i], LOW);
        actualPower[i] = 255;
      break;
      case 2: //go forwards
        wantedPower[i] = motorPower[i] + 255;
        
        if (actualPower[i] - wantedPower[i] < 2 && actualPower[i] - wantedPower[i] > -2){
           actualPower[i] = wantedPower[i]; 
        }  
        else if (actualPower[i] < wantedPower[i]){
          actualPower[i] = actualPower[i] + 2;
        }
        else if (actualPower[i] > wantedPower[i]){
          actualPower[i] = actualPower[i] - 2;
        }
        if (actualPower[i] > 254) {
          analogWrite(motorEnablePin[i], actualPower[i] - 255);
          digitalWrite(motorFPin[i], HIGH);
          digitalWrite(motorBPin[i], LOW);
        }
        else {
          analogWrite(motorEnablePin[i], 255 - actualPower[i]);
          digitalWrite(motorFPin[i], LOW);
          digitalWrite(motorBPin[i], HIGH);
        }  
      break;
      case 3: // backwards
        wantedPower[i] = 255 - motorPower[i];
        
        if (actualPower[i] - wantedPower[i] < 2 && actualPower[i] - wantedPower[i] > -2){
           actualPower[i] = wantedPower[i]; 
        }  
        else if (actualPower[i] < wantedPower[i]){
          actualPower[i] = actualPower[i] + 2;
        }
        else if (actualPower[i] > wantedPower[i]){
          actualPower[i] = actualPower[i] - 2;
        }
        if (actualPower[i] > 254) {
          analogWrite(motorEnablePin[i], actualPower[i] - 255);
          digitalWrite(motorFPin[i], HIGH);
          digitalWrite(motorBPin[i], LOW);
        }
        else {
          analogWrite(motorEnablePin[i], 255 - actualPower[i]);
          digitalWrite(motorFPin[i], LOW);
          digitalWrite(motorBPin[i], HIGH);
        }  
      break;
      default: //if mode is set to something else then turn it off
        analogWrite(motorEnablePin[i], 0);
        digitalWrite(motorFPin[i], LOW);
        digitalWrite(motorBPin[i], LOW);
      break;
}
}
//  Serial.println("still here");
  }
}

void receiveEvent(int howMany)
{
   while(0 < Wire.available())
  {
     byte x = Wire.read();
 //    Serial.print("Byte received: ");
//     Serial.println(x); 
     int motorNum = (x >> 5);
//     Serial.print("Motor num: "); 
//     Serial.println(motorNum);
 //      Serial.println(x & 16);
 //    if ( (x & 16) == 16){
//     Serial.print("Motor Mode: "); 
//       Serial.println(int((x&6)>>1));
//     Serial.print("Motor Mode: "); 
//     Serial.println(int((x&6)>>1));
  //   }
  motorMode[motorNum] = int((x&6)>>1);
    if (((x & 8) == 8) && (Wire.available() > 0)){
      motorPower[motorNum] = int(Wire.read());
//      Serial.print("Motor power: "); 
//      Serial.println(motorPower[motorNum]);
    }
    if ((x & 1) == 1){
       stopAll = 1;
    }
  } 
}
