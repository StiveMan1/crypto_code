#ifndef AES_H
#define AES_H

#include <malloc.h>
#include <string.h>

char *aesEncode(const char msg[], size_t size, const char key[], size_t *res_size);
char *aesDecode(const char plain_text[], size_t size, const char key[], size_t *res_size);


#endif //AES_H
