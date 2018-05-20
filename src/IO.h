#ifndef IO_H
#define IO_H

#include <string>

namespace IO
{
    std::string GetTargetLogsDirPath(const bool append_seperator);
    bool CheckFileExists(const std::string &f);
    bool CheckDirExists(const std::string &dir);
    bool DeleteFile(const std::string &f);
    bool MakeOneDirectory(std::string path);
    bool MKDir(std::string path);
    void WriteDebugAppLog(const std::string &s);

    std::string GetThisFilePath();
    std::string GetCurDir();

    void WriteLog( const std::string &s);
    std::string GetLastLogPath();
    bool GetLogWriteSuccess();

    void testWrite();

} // namespace IO

#endif // IO_H