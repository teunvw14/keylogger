#include <vector>
#include <string>
#include "encryption.h"



namespace Base64
{

    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string &stringToEncode)
    {
        std::string ret;
        int val = 0;
        int bits = -6;
        const unsigned int b63 = 0x3F;

        std::string encryptedString = "";
        for(const auto &character : stringToEncode)
        {
            val = (val << 8) + character;
            bits += 8;
            while(bits >= 0)
            {
                ret.push_back(BASE64_CODES[(val >> bits) & b63]);
                bits -= 6;
            }
        }
        if (bits > -6)
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

        while(ret.size() % 4)
            ret.push_back('=');
        return ret;
    }    
}

namespace Encryption
{
    
    std::string EncryptB64( std::string s)
    {
        std::string SALT1 = "DZ6k6uarM";
        std::string SALT2 = "Hiqn0jvTf";
        std::string SALT3 = "IPjf56y8";

        s = SALT1 + s + SALT2 + SALT3;
        
        s = Base64::base64_encode(s);
        s.insert(7, SALT2);
        s += SALT1;
        s = Base64::base64_encode(s);
        s = SALT2 + SALT1 + s;
        s = Base64::base64_encode(s);
        s.insert(1, "m");
        s = Base64::base64_encode(s);

        return s;
    }

}