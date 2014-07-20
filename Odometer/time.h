


class Time{
      public:
             int getHour();
             int getMinute();
             double getSecond();
             void setTime(int newHour, int newMinute, double newSecond);
             void print();
             void printTime();
             double getDifference(Time toCompare);
             void setTimeEqualTo(Time toSet);
             int findBiggerTime(Time toFind);
             int hourToMinute(int yHour);
             int minuteToSecond(int yMinute);
             void setTime(int newHour, int newMinute, int newSecond);
             //Time(int newHour, int newMinute, double newSecond): hour(newHour), second(newSecond), mintue(newMinute);
      private:
              int hour;
              int minute;
              double second;
              enum{SELF, OTHER, ERROR, SAME};
      };
