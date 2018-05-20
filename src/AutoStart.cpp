#include <windows.h>
#include <string>
#include "IO.h"


namespace AutoStart
{
    bool setAutoRun( const std::string &name, const std::string &val)
    {
        HKEY hKey;
        LPCTSTR sk = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

        if (RegOpenKey(HKEY_CURRENT_USER, sk, &hKey) != ERROR_SUCCESS)
        {
            //IO::WriteDebugAppLog("Could not open regex to enable autorun...");
            return false;
        }

        const LPCTSTR entryName = name.c_str();
        const LPCTSTR entryValue = val.c_str();

        if (RegSetValueEx(hKey, entryName, 0, REG_SZ, (LPBYTE)entryValue, val.size() * sizeof(char)) != ERROR_SUCCESS)
        {
            //IO::WriteDebugAppLog("Could not set values in regex to enable autorun...");
            return false;
        }
        RegCloseKey(hKey);
        return true;
    }
}