#include <string>
#include <vector>


namespace Decryption
{

    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_decode(const std::string &s)
    {
        std::string ret;
        std::vector<int> vec(256, -1);
        for (int i = 0; i < 64; i++)
            vec[BASE64_CODES[i]] = i;
        int val = 0, bits = -8;
        for (const auto &c : s)
        {
            if (vec[c] == -1)
                break;
            val = (val << 6) + vec[c];
            bits += 6;

            if (bits >= 0)
            {

                ret.push_back(char((val >> bits) & 0xFF));
                bits -= 8;
            }
        }

        return ret;
    }

    std::string DecryptB64(std::string s)
    {

        std::string SALT1 = "DZ6k6uarM";
        std::string SALT2 = "Hiqn0jvTf";
        std::string SALT3 = "IPjf56y8";

        std::string decS = s;

        decS = base64_decode(decS);
        s = decS.erase(1, 1);
        decS = base64_decode(decS);
        decS = decS.erase(0, SALT1.length() + SALT2.length());
        decS = base64_decode(decS);
        decS = decS.erase(decS.length() - SALT1.length(), SALT1.length());
        decS = decS.erase(7, SALT2.length());
        decS = base64_decode(decS);
        decS = decS.erase(0, SALT1.length());
        decS = decS.erase(decS.length() - SALT2.length() - SALT3.length(), SALT2.length() + SALT3.length());

        return decS;
    }
} // Decryption


