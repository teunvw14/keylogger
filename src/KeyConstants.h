#ifndef KEYCONSTANTS_H
#define KEYCONSTANTS_H

#include <string>
#include <map>

class KeyPair
{
    public:
        KeyPair (const std::string &vk, const std::string &name, const std::string &shiftname);
        std::string VKName;
        std::string Name;
        std::string ShiftName;
};

class Keys
{
    public:
        static std::map<int, KeyPair> KEYS;
};

#endif