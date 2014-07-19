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

#define PRINT_MAIN_EVERY 5000
#define PRINT_LAP_EVERY 15000

// variable defitions
volatile int currentRevolutions = 0;


unsigned long startTime = 0; // 
unsigned long endTime = 0;
unsigned long loopNum = 0;

const float tireDiameter = .55; // diameter in m of bike tire
const float MSTOKMH = 3.6; // constant for meters per second to km/h

float totalDistance = 0;
float previousDistance = 0;
float totalTime = 0;
Time rideTime;
Time lapTime;

float thisDistance = 0;
float thisVelocity = 0;
float thisTime = 0;

//pin definitions
char wheelPin = 2;
char LEDpin = 13;
char speakerPin = 3;
char resetButton = 12;
char lapButton = 11;

unsigned int lapTimes[20];
unsigned int lapDistances[20];
unsigned char lapsCompleted;


void setup(){
  rideTime.setTime(0,0,0);
  Serial.begin(9600); //begin serial communication
  pinMode(wheelPin, INPUT); // set wheelPin to input
  pinMode(LEDpin, OUTPUT); //Pin for LED
  pinMode(speakerPin, OUTPUT);
  pinMode(resetButton, INPUT);
  digitalWrite(resetButton, HIGH); // enable internal pull up resistors
  pinMode(lapButton, INPUT); 
  digitalWrite(lapButton, HIGH); // enable internal pull up resistors

 
  
  
  attachInterrupt(0, wheelSpin, RISING); //interrupt for each spin of the wheels.

  /* Timer2 setup stuff */
  TCCR2B = ( _BV(CS22) | _BV(CS21) | _BV(CS20)); // set Timer 2 prescalar to 1024
  TIMSK2 |= (1<<TOIE2); // enable timer2, given value from datasheet
  TIMSK1 |= (1<<TOIE1); // enable timer0, given value from datasheet
}

void loop(){
  // set the current time
  rideTime.setTime(0,0,myMillis()/1000); // set current ride time based on the myMillis function
  lapTime.setTime(0,0,currentLapTime()-lapTimes[lapsCompleted]); // set current lap time based on the other millis function


  noTone(speakerPin); // get rid of any current playing tones

  if(digitalRead(resetButton) == LOW){ // if we are reseting lap data
    // print all lap data here????
      resetLapTimer();
      lapsCompleted = 0;
      for(int i = 0; i < 20; i++){
        lapTimes[i] = 0;
        lapDistances[i]=0;
      }
      lapDistances[0] = totalDistance;
  }
  else if(digitalRead(lapButton) == LOW){
    //save current lap time
    lapTimes[lapsCompleted+1] = currentLapTime() - lapTimes[lapsCompleted];
    lapDistances[lapsCompleted+1] = totalDistance - lapDistances[lapsCompleted];
    // add one to lapsCompleted
    lapsCompleted++;
  }
  
  usrTotalTime= myMillis()/ 1000;
  //get avg speed over this time interval
  if(loopNum%PRINT_MAIN_EVERY == 0) { // every 1000 main loops, print main values
    printMainValues(); 
  }
  if(loopNum % PRINT_LAP_EVERY == 0){ // every 3000 main loops, print lap values
    printLapValues(); 
  }

  loopNum++;
}

void printLapValues(){
  Serial.print("Current Lap Number: ");
  Serial.println(lapsCompleted);
  Serial.print("Current Lap Time: ");
  Serial.println(currentLapTime() - lapTimes[lapsCompleted]);
  Serial.print("Current Lap Distance: ");
  Serial.println(totalDistance - lapDistances[lapsCompleted]);
} 


void printMainValues(){

  // ********debug code**************
  //Serial.println(previousDistance, 10);
  //Serial.println(totalDistance, 10);
  //***********END debug code END********

  Serial.print("Current Velocity is: ");
  Serial.print(thisVelocity);
  Serial.println(" km/h");
  Serial.print("Total Distance travelled is: ");
  Serial.print(totalDistance);
  Serial.println(" km");
  Serial.print("Total time for this ride has been: ");
  Serial.print(rideTime.printTime());
  Serial.println(" seconds");
  Serial.print("Average speed for this ride has been: ");
  Serial.print(computeAvgVel());
  Serial.println(" km/h");
  Serial.println("");
}

float computeAvgVel(){
  return (totalDistance/totalTime)*MSTOKMH;
} 


void comuputeValues(){
  endTime = myMillis();

  //compute current distance, time, and velocity
  thisDistance = (currentRevolutions*PI)*tireDiameter;
  thisTime = (endTime - startTime)/1000.0;
  startTime = myMillis();

  thisVelocity = (thisDistance / thisTime)*MSTOKMH ;
  
  //add current values to totals
  float lastDistance = totalDistance;
  totalDistance += thisDistance/1000.0;
  checkDistanceForNewKm(lastDistance, thisDistance); //What should this do?
 
  totalTime += thisTime;
  
  //reset current variables
  thisTime = 0;
  thisDistance = 0;
  currentRevolutions = 0;
}

void checkDistanceForNewKm(float lastDistance, float thisDistance){
  int thisDistance_trunk = thisDistance;
  int lastDistance_trunk = lastDistance;
  if(thisDistance_trunk > lastDistance_trunk){
    //we have hit a km mark, sound the allarms!
    tone(speakerPin, 500, 200);
  }
}

void wheelSpin(){
  currentRevolutions++;
  comuputeValues();
}