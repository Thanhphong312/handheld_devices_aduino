#ifndef RTCDS3231_h
#define RTCDS3231_h
#include <DS3231.h>
using namespace std;

class RTCDS3231{
  private:
    int sda = 20;
    int scl = 21;
    int hour;
    int second;
    int minute;
    int day;
    int month;
    int year;
    DS3231 rtc{sda, scl};
  public:
    RTCDS3231(){

    }
    ~RTCDS3231(){

    }
    void setRTC(){
      DS3231 newrtc{this->sda, this->scl};
      this->rtc = newrtc;
      this->rtc.begin();
    }
    void setDefaultTime(char* DOW, int hour, int minute, int second, int day, int month, int year){
      rtc.setTime(hour, minute, second);     // Set the time to 12:00:00 (24hr format)
      rtc.setDate(day, month, year);
    }
    char *getRTCTimeStr(){
      return this->rtc.getTimeStr();
    }
    char *getRTCDateStr(){
      return this->rtc.getDateStr();
    }
    void setArrayTime(){
      int h, m, s;
      sscanf(this->rtc.getTimeStr(), "%d:%d:%d", &h, &m, &s);
      this->hour = h;
      this->minute = m;
      this->second = s;
      int d, mm, y;
      sscanf(this->rtc.getDateStr(), "%d.%d.%d", &d, &mm, &y);
      this->day = d;
      this->month = mm;
      this->year = y;
    }
    int getHour(){
      return this->hour;
    }
    int getSecond(){
      return this->second;
    }
    int getMinute(){
      return this->minute;
    }
    int getDay(){
      return this->day;
    }
    int getMonth(){
      return this->month;
    }
    int getYear(){
      return this->year;
    }
};
#endif