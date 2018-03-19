#include "includes/Date.h"

//statische Variablen
time_t Date::timestamp;
tm* Date::now;
stringstream Date::converter;

string Date::get_time(int flag = 0)
{
    string sztime = "";

    fillTime();

    if(now->tm_hour < 10) sztime += '0';
    sztime += IntToStr(now->tm_hour) + ':';
    if(now->tm_min < 10) sztime += '0';
    sztime += IntToStr(now->tm_min);
    if (flag == 1)
    {
        sztime += ':';
        if(now->tm_sec < 10) sztime += '0';
        sztime +=  IntToStr(now->tm_sec);
    }
    return sztime;
}

string Date::get_date()
{
    string date;

    fillTime();

    date = IntToStr(now->tm_mday) + '.';
    date += IntToStr(now->tm_mon) + '.';
    date += IntToStr(now->tm_year + 1900);

    return date;
}

void Date::fillTime()
{
    timestamp = time(0);
    now = localtime(&timestamp);
}

string Date::IntToStr(int i)
{
    string tmp;
    converter.clear();
    converter << i;
    converter >> tmp;
    return tmp;
}
