#include "crypto_code.h"

char *encode(const char msg[], const size_t size, const char key[], const int type, size_t* res_size) {
    if (type == AES_256) {
        return aesEncode(msg, size, key, res_size);
    } else {
        printf("Not implemented crypto algo\n");
//        throw;
    }
    return NULL;
}

char *decode(const char plain_text[], const size_t size, const char key[], const int type, size_t* res_size) {
    if (type == AES_256) {
        return aesDecode(plain_text, size, key, res_size);
    } else {
        printf("Not implemented crypto algo\n");
    }
    return NULL;
}
