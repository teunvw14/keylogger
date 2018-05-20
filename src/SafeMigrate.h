#include <string>

namespace ExeMigration
{
    bool hasEnding(std::string const &fullString, std::string const &ending);

    bool MigrateToSafeDir();
    bool CheckIfSafe();
    std::string GetSafePath();
    bool CreateMigrationScript();
    void DeleteMigrationScript();
} // ExeMigration
