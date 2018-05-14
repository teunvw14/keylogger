#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
    public:

        static bool InitSettings();
        
        const static std::string mail_from;
        static std::string get_mail_from();

        const static std::string mail_pass;
        static std::string get_mail_pass();

        const static std::string mail_to;
        static std::string get_mail_to();

        const static unsigned long mail_interval;
        static unsigned long get_mail_interval();

        const static bool useVerboseLogs;
        static bool get_useVerboseLogs();
};

#endif