#include <stdlib.h>


#include "time.h"
#include <stdlib.h>
#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <wiring.h>
#endif

int Time::getHour(){
    return hour;
}

int Time::getMinute(){
    return minute;
}
double Time::getSecond(){
    return second;
}
void Time::setTime(int newHour, int newMinute, int newSecond){
    minute = 0;
    hour = 0;
    second = 0;
    if(newSecond >= 60){
        second += newSecond%60;
        newSecond -= second;
        minute = (newSecond/60);
    }
    else 
        second = newSecond;

    minute += newMinute;
    if(minute >= 60){
        int oldMinute = minute;
        minute = oldMinute%60;
        oldMinute -= minute;
        hour = (oldMinute/60);
    }
    hour += newHour;
    
}

void Time::printTime(){
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
}

double Time::getDifference(Time toCompare){
    int rMinute = 0, rHour = 0;
    double rSecond = 0;
    int bigger = findBiggerTime(toCompare);
    if(bigger == SELF){
              rHour = hour - toCompare.getHour();
              rMinute = minute - toCompare.getMinute();
              rSecond = second - toCompare.getSecond();
    }
    else{
         rHour = toCompare.getHour() - hour;
         rMinute = toCompare.getMinute() - minute;
         rSecond = toCompare.getSecond() - second;
    }
    
    //convert difference to seconds
    rSecond += minuteToSecond(hourToMinute(rHour));
    rSecond += minuteToSecond(rMinute);
    
    return rSecond;
}


void Time::setTimeEqualTo(Time toSet){
     hour = toSet.getHour();
     minute = toSet.getMinute();
     second = toSet.getSecond();
}
int Time::findBiggerTime(Time toFind){
    if(hour == toFind.getHour()){
            if (minute == toFind.getMinute()){
                       if(second > toFind.getSecond()){
                                 return SELF;
                       }
                       else if (toFind.getSecond() > second){
                            return OTHER;
                       }
                       else return SAME;
            }
            else if(minute > toFind.getMinute()){
                 return SELF;
            }
            else return OTHER;
    }
    else if( hour > toFind.getHour()){
         return SELF;
    }
    else return OTHER;
}

int Time::hourToMinute(int yHour){
    return yHour *60;
    
}
int Time::minuteToSecond(int yMinute){
    return yMinute * 60;
}
