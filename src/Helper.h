#ifndef HELPER_H
#define HELPER_H

#include <string>

namespace Helper
{
    template <class T>
    std::string ToString(const T &e);
    
    std::string LongToString(const long &l);

    std::string ToUpper(const std::string &s);

    struct DateTime
    {
        int year, month, day, hour, minute, second;
        DateTime();
        DateTime(int yr, int mon, int dy, int hr, int min, int sec);
        DateTime(int yr, int mon, int dy);

        DateTime Now() const;
        
        std::string GetDateString() const;
        std::string GetTimeString(const std::string &sep) const;
        std::string GetDateTimeString(const std::string &tsep, const std::string &dtsep) const;
    };
};

#endif