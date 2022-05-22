#include "crypto_code.h"
#include "aes.h"

std::string crypto::encode(const std::string &msg, const std::string &key, const int type) {
    switch (type) {
        case AES_256:
            return crypto::aes::encode(msg, key);
    }
    throw WrongType();
}

std::string crypto::decode(const std::string &plain_text, const std::string &key, const int type) {
    switch (type) {
        case AES_256:
            return crypto::aes::decode(plain_text, key);
    }
    throw WrongType();
}

std::string crypto::crypto_type::encode(const std::string &msg, const std::string &key) {
    throw NotRepresentedType();
}

std::string crypto::crypto_type::decode(const std::string &msg, const std::string &key) {
    throw NotRepresentedType();
}
