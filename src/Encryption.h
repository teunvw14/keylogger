#ifndef ENCODING_H
#define ENCODING_H

#include <string>

namespace Base64
{
    std::string base64_encode(const std::string &stringToEncode);
}

namespace Encryption
{
    std::string EncryptB64(std::string s);
}


#endif //ENCODING_H