///////////////// 1. LIGHTS WITH MOTION SENSOR /////////////////
//The time the sensor gets to calibrate
int calibrationTime = 10;       
 
//The time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//The amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 3;
int ledPin = 13;
 
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
 
  //Calibrating sensor
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }
 
void loop(){

     //If there has been detected motion
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);
       if(lockLow){ 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         delay(50);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPin, LOW);  
 
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause,
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){ 
           //makes sure this block of code is only executed again after
           //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
  }
  
///////////////// 2. DOOR OPEN/CLOSING WITH SERVO MOTOR /////////////////
#include <Servo.h>

const byte button = 4; //button pin, connect to ground to move servo

Servo servo;

boolean buttonState = HIGH;
boolean lastButtonState = HIGH;
int pos = 0;

void setup()
{
   pinMode(button, INPUT_PULLUP); //arduino monitor pin state
   servo.attach(9); //pin for servo control signal
}

void loop()
{
   buttonState = digitalRead(button);
   if (buttonState != lastButtonState)
   {
      if (buttonState == LOW)
      {
        //Open the door
        for(pos = 80; pos <= 180; pos +=1){
          servo.write(pos);
          delay(15);
        }
         //A brake between opening en closing the door
         delay(3000);  

        //Closing the door
        for(pos = 180; pos >= 80; pos -=1){
          servo.write(pos);
          delay(15);
        }
      } 
      lastButtonState = buttonState;
   }
}
