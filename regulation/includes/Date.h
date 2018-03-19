#ifndef _DATE_H_
#define _DATE_H_

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

class Date
{
  private:
    static time_t timestamp;
    static tm* now;
    static stringstream converter;

  public:
    static string get_time(int flag);
    static string get_date();
    static void fillTime();
    static string IntToStr(int i);
};



#endif
