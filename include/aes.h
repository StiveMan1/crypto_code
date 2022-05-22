#ifndef AES_H
#define AES_H

#include "crypto_code.h"

namespace crypto{
    class aes : virtual crypto_type{
    public:
        static std::string encode(const std::string &msg, const std::string &key);
        static std::string decode(const std::string &plain_text, const std::string &key);
    };
}

#endif //AES_H
