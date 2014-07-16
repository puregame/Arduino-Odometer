// CREATED BY:  Matthew Gougeon, Andre Chen and Johnie Devries??
// Arduino based bycicle odometer
#include <stdlib.h>
#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <wiring.h>
#endif

#include "myMillis.h"
#include "time.h"


volatile int currentRevolutions = 0;

long startTime = 0; // 
long endTime = 0;

const float pi = 3.141592653;
const float tireDiameter = .55; // diameter in m
const float MSTOKMH = 3.6;

float totalDistance = 0;
float previousDistance = 0;
float totalTime = 0;
long usrTotalTime = 0;

float thisDistance = 0;
float thisVelocity = 0;
float thisTime = 0;


//pin definitions
int inPin = 2;
int LEDpin = 13;
int speakerPin = 3;

Time lapTimes[20];


void setup(){
  Serial.begin(9600); //begin serial communication
  pinMode(inPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  TCCR2B = ( _BV(CS22) | _BV(CS21) | _BV(CS20)); // set Timer 2 prescalar to 1024
  
  // should this be rising??
  attachInterrupt(0, wheelSpin, RISING); //interrupt for each spin of the wheels.
}

void loop(){
  previousDistance = totalDistance;
  delay(1000);
  //do shit here
  usrTotalTime+=1;
  //get avg speed over this time interval
  
  printValues();
  
}


void printValues(){
  Serial.println(previousDistance, 10);
  Serial.println(totalDistance, 10);
  Serial.print("Current Velocity is: ");
  Serial.print(thisVelocity);
  Serial.println(" km/h");
  Serial.print("Total Distance travelled is: ");
  Serial.print(totalDistance);
  Serial.println(" km");
  Serial.print("Total time for this ride has been: ");
  Serial.print(usrTotalTime);
  Serial.println(" seconds");
  Serial.print("Average speed for this ride has been: ");
  Serial.print(computeAvgVel());
  Serial.println(" km/h");
  Serial.println("");
}

float computeAvgVel(){
  return (totalDistance/totalTime)*MSTOKMH;
} 

void checkDistances(){
  
  if(previousDistance < 1 && totalDistance > 1){
    tone(speakerPin, 900);
    delay(500);
    noTone(speakerPin);
  }
}

void comuputeValues(){
  endTime = millis();
  //compute current distance, time, and velocity
  thisDistance = (currentRevolutions*pi)*tireDiameter;
  thisTime = (endTime - startTime)/1000.0;
  startTime = millis();
  thisVelocity = (thisDistance / thisTime)*MSTOKMH ;
  
  //add current values to totals
  totalDistance += thisDistance/1000.0;
  checkDistances();
  
  totalTime += thisTime;
  
  //reset current variables
  thisTime = 0;
  thisDistance = 0;
  currentRevolutions = 0;
}

void wheelSpin(){
  currentRevolutions++;
  comuputeValues();
}

