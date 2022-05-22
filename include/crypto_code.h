#ifndef CRYPTO_CODE_H
#define CRYPTO_CODE_H

#define AES_256 1

#include <string>

namespace crypto {

    class NotRepresentedType : public std::exception {
    };
    class WrongType : public std::exception {
    };

    class crypto_type {
    public:
        static std::string encode(const std::string &msg, const std::string &key);
        static std::string decode(const std::string &msg, const std::string &key);
    };


    std::string encode(const std::string &msg, const std::string &key, int type);

    std::string decode(const std::string &plain_text, const std::string &key, int type);

}
#endif //CRYPTO_CODE_H
