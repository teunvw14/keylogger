#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "Helper.h"

namespace Helper
{
    template <class T>
    // function for converting any type of input to a string
    std::string ToString(const T &e)
    {
        std::ostringstream outputStringStream;
        outputStringStream << e;
        return outputStringStream.str();
    }


    std::string LongTostring(const long &l)
    {
        std::ostringstream outputStringStream;
        outputStringStream << l;
        return outputStringStream.str();
    }


    std::string ToUpper(const std::string &s)
    {
        std::string lowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
        std::string upperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        
        std::string tempString = "";

        for ( char c : s )
        {
            tempString += upperCaseLetters.at(lowerCaseLetters.find(c));
        }
        
        return tempString;
    }

    //default constructor that automatically generates datetime that represents the current date and time
    DateTime::DateTime()
    {
        time_t ms;
        time(&ms);
        struct tm *info = localtime(&ms);
        year = 1900 + info->tm_year;
        month = info->tm_mon + 1;
        day = info->tm_mday;
        hour = info->tm_hour;
        minute = info->tm_min;
        second = info->tm_sec;
    }
    //constructor that takes year, month, day, hour, minute and second as input
    DateTime::DateTime(int yr, int mon, int dy, int hr, int min, int sec) : year(yr),
                                                                  month(mon),
                                                                  day(dy),
                                                                  hour(hr),
                                                                  minute(min),
                                                                  second(sec)
                                                                  {}
    //constructor that takes year, month and day as input and sets time to 00:00:00
    DateTime::DateTime(int yr, int mon, int dy) : year(yr),
                                        month(mon),
                                        day(dy),
                                        hour(0),
                                        minute(0),
                                        second(0)
                                        {}
    
    //function that returns date of DateTime object as a string
    std::string DateTime::GetDateString() const
    {
        return std::string(day < 10 ? "0" : "") + ToString(day) + "/" +
               std::string(month < 10 ? "0" : "") + ToString(month) + "/" +
               ToString(year);
    }
    //function that returns time of DateTime object as a string
    std::string DateTime::GetTimeString(const std::string &sep = ".") const
    {
        return std::string(hour < 10 ? "0" : "") + ToString(hour) + sep +
               std::string(minute < 10 ? "0" : "") + ToString(minute) + sep +
               std::string(second < 10 ? "0" : "") + ToString(second);
    }
    //function that returns date and time of DateTime object as a string
    std::string DateTime::GetDateTimeString(const std::string &tsep = ".", const std::string &dtsep = " | ") const
    {
        return GetDateString() + dtsep + GetTimeString(tsep);
    }
    
}; // Helper
