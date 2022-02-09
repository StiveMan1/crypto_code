#ifndef CRYPTO_CODE_H
#define CRYPTO_CODE_H

#define AES_256 1

#include "aes.h"


char *encode(const char msg[], size_t size, const char key[], int type, size_t* res_size);

char *decode(const char plain_text[], size_t size, const char key[], int type, size_t* res_size);


#endif //CRYPTO_CODE_H
