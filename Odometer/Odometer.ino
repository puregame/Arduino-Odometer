// CREATED BY:  Matthew Gougeon, Andre Chen and Jonathan Devries
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
#define LAPS_SUPPORTED 20

// variable defitions
volatile int currentRevolutions = 0;


unsigned long startTime = 0; // 
unsigned long endTime = 0;
unsigned long loopNum = 0;

const float tireDiameter = .55; // diameter in m of bike tire
const float MSTOKMH = 3.6; // constant for meters per second to km/h

float totalDistance = 0;
float previousDistance = 0;
float usrTotalTime = 0;
Time rideTime;
Time lapTime;

float thisDistance = 0;
float thisVelocity = 0;
float thisTime = 0;

//pin definitions
char wheelPin = 2;
char LEDpin = 13;
char speakerPin = 5;
char resetButton = 12;
char lapButton = 11;

// lap counter data structures for LAPS_SUPPORTED laps, this value can be increased if needed
 float lapTimes[LAPS_SUPPORTED];
 float lapDistances[LAPS_SUPPORTED];
unsigned char lapsCompleted = 0;


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

  attachInterrupt(1, wheelSpin, RISING); //interrupt for each spin of the wheels.

  /* Timer 1 and 2 setup stuff */
  TIMSK1 |= (1<<TOIE1); // enable timer0, given value from datasheet
}

void loop(){
  // set the current time
  rideTime.setTime(0,0, int(myMillis()/1000)); // set current ride time based on the myMillis function
  lapTime.setTime(0,0, int(currentLapTime()-lapTimes[lapsCompleted])); // set current lap time based on the other millis function


  noTone(speakerPin); // get rid of any current playing tones

  if(digitalRead(resetButton) == LOW){ // if we are reseting ALL lap data
    // print all lap data here???? this could be a future feature
      resetLapTimer(); // sets timer2millis to 0
      lapsCompleted = 0;
      for(int i = 0; i < 20; i++){ // reset all lap values
        lapTimes[i] = 0;
        lapDistances[i]=0;
      }
      lapDistances[0] = totalDistance; // reset initial lap distance to current distance travelled
  }
  if(digitalRead(lapButton) == LOW){ // if we want to create a new lap
    //save current lap time
    lapTimes[lapsCompleted] = currentLapTime();// - lapTimes[lapsCompleted];
    //save current lap distance
    lapDistances[lapsCompleted] = totalDistance;// - lapDistances[lapsCompleted];
    // add one to lapsCompleted
    lapsCompleted++;
  }
  if( digitalRead(lapButton) == LOW || digitalRead(resetButton) == LOW){
    //debounce these buttons
    delay(200);
  }
  
  usrTotalTime= myMillis()/ 1000; // set main clock based on my millis function

  //get avg speed over this time interval
  if(loopNum%PRINT_MAIN_EVERY == 0) { // every PRINT_MAIN_EVERY main loops, print main values
    printMainValues();
  }
  if(loopNum % PRINT_LAP_EVERY == 0 && lapsCompleted > 0){ // every PRINT_LAP_EVERY main loops, print lap values
    printLapValues(); 
  }

  loopNum++;
}

void printLapValues(){
  Serial.println("");
  Serial.print("Current Lap Number: ");
  Serial.println(lapsCompleted);
  Serial.print("Current Lap Time: ");
  Serial.println(currentLapTime() - lapTimes[lapsCompleted-1]);
  Serial.print("Current Lap Distance: ");
  Serial.println(totalDistance - lapDistances[lapsCompleted-1]);
} 


void printMainValues(){
  Serial.println("");
  Serial.print("Current Velocity is: ");
  Serial.print(thisVelocity);
  Serial.println(" km/h");
  Serial.print("Total Distance travelled is: ");
  Serial.print(totalDistance);
  Serial.println(" km");
  Serial.print("Total time for this ride has been: ");
  rideTime.printTime();
  Serial.println(" hours:minutes:seconds");
  Serial.print("Average speed for this ride has been: ");
  Serial.print((totalDistance/float(myMillis()/1000)) * 3600.0);
  Serial.println(" km/h");
  Serial.println("");
}

//NEEDS TO BE FIXED
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
  if(currentRevolutions >= 5)
    comuputeValues();
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
  /*
  Serial.println("This stuff:");
  Serial.print("thisVelocity = ");
  Serial.println(thisVelocity);
  Serial.print("thisDistance = ");
  Serial.println(thisDistance);
  Serial.print("thisTime = ");
  Serial.println(thisTime);*/

  //reset current variables
  thisTime = 0;
  thisDistance = 0;
  currentRevolutions = 0;
}
