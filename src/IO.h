#ifndef IO_H
#define IO_H

#include <string>

namespace IO
{
    std::string GetTargetLogsDirPath(const bool append_seperator);
    bool CheckFileExists(const std::string &f);
    bool DeleteFile(const std::string &f );
    bool MakeOneDirectory(std::string path);
    bool MKDir(std::string path);
    void WriteDebugAppLog(const std::string &s);

    void WriteToLog(const std::string &stringToWrite);

    std::string GetLastLogPath();
    bool GetLogWriteSuccess();

} // namespace IO

#endif // IO_H