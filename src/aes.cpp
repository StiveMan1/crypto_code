#include <cstring>
#include "aes.h"


typedef struct {
    int Nb;
    int Nk;
    int Nr;
    unsigned blockBytesLen;
} aes_base;
typedef struct {
    unsigned char *ctx;
    unsigned length;
} aes_ctx;

const unsigned char sbox[16][16] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
        0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
        0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
        0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
        0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
        0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
        0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
        0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
        0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
        0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
        0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
        0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
        0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
        0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
        0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
        0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
        0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const unsigned char inv_sbox[16][16] = {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
        0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
        0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
        0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
        0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
        0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
        0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
        0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
        0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
        0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
        0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
        0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
        0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
        0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
        0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
        0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
        0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,};


unsigned char *PaddingNulls(unsigned char in[], unsigned inLen, unsigned alignLen) {
    auto *alignIn = (unsigned char *) malloc(sizeof(unsigned) * alignLen);
    memcpy(alignIn, in, inLen);
    memset(alignIn + inLen, 0x00, alignLen - inLen);
    return alignIn;
}

unsigned GetPaddingLength(unsigned len, aes_base *aes) {
    unsigned lengthWithPadding = (len / aes->blockBytesLen);
    if (len % aes->blockBytesLen) {
        lengthWithPadding++;
    }

    lengthWithPadding *= aes->blockBytesLen;
    return lengthWithPadding;
}


void SubBytes(unsigned char **state, aes_base *aes) {
    int i, j;
    unsigned char t;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            t = state[i][j];
            state[i][j] = sbox[t / 16][t % 16];
        }
    }
}

void ShiftRow(unsigned char **state, int i, int n, aes_base *aes) {
    auto *tmp = (unsigned char *) malloc(aes->Nb * sizeof(unsigned char));
    for (
            int j = 0;
            j < aes->Nb;
            j++) {
        tmp[j] = state[i][(j + n) % aes->Nb];
    }
    memcpy(state[i], tmp, aes->Nb
                          * sizeof(unsigned char));
    free(tmp);
}

void ShiftRows(unsigned char **state, aes_base *aes) {
    ShiftRow(state, 1, 1, aes);
    ShiftRow(state, 2, 2, aes);
    ShiftRow(state, 3, 3, aes);
}

unsigned char xtime(unsigned char b) {
    return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}


void MixSingleColumn(unsigned char *r) {
    unsigned char a[4];
    unsigned char b[4];
    unsigned char c;
    unsigned char h;
    for (c = 0; c < 4; c++) {
        a[c] = r[c];

        h = (unsigned char) ((signed char) r[c]
                >> 7);
        b[c] = r[c]
                << 1;
        b[c] ^= 0x1B & h;
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}


void MixColumns(unsigned char **state) {
    auto *temp = (unsigned char *) malloc(sizeof(unsigned) * 4);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[j] = state[j][i];
        }
        MixSingleColumn(temp);
        for (int j = 0; j < 4; ++j) {
            state[j][i] = temp[j];
        }
    }
    free(temp);
}

void AddRoundKey(unsigned char **state, const unsigned char *key, aes_base *aes) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            state[i][j] = state[i][j] ^ key[i + 4 * j];
        }
    }
}

void SubWord(unsigned char *a) {
    int i;
    for (i = 0; i < 4; i++) {
        a[i] = sbox[a[i] / 16][a[i] % 16];
    }
}

void RotWord(unsigned char *a) {
    unsigned char c = a[0];
    a[0] = a[1];
    a[1] = a[2];
    a[2] = a[3];
    a[3] = c;
}

void XorWords(const unsigned char *a, const unsigned char *b, unsigned char *c) {
    int i;
    for (i = 0; i < 4; i++) {
        c[i] = a[i] ^ b[i];
    }
}

void Rcon(unsigned char *a, int n) {
    int i;
    unsigned char c = 1;
    for (i = 0; i < n - 1; i++) {
        c = xtime(c);
    }

    a[0] = c;
    a[1] = a[2] = a[3] = 0;
}

void KeyExpansion(const unsigned char key[], unsigned char w[], aes_base *aes) {
    auto *temp = (unsigned char *) malloc(sizeof(unsigned char) * 4);
    auto *rcon = (unsigned char *) malloc(sizeof(unsigned char) * 4);

    int i = 0;
    while (i < 4 * aes->Nk) {
        w[i] = key[i];
        i++;
    }

    i = 4 * aes->Nk;
    while (i < 4 * aes->Nb * (aes->Nr + 1)) {
        temp[0] = w[i - 4 + 0];
        temp[1] = w[i - 4 + 1];
        temp[2] = w[i - 4 + 2];
        temp[3] = w[i - 4 + 3];

        if (i / 4 % aes->Nk == 0) {
            RotWord(temp);
            SubWord(temp);
            Rcon(rcon, i / (aes->Nk * 4));
            XorWords(temp, rcon, temp);
        } else if (aes->Nk > 6 && i / 4 % aes->Nk == 4) {
            SubWord(temp);
        }

        w[i + 0] = w[i - 4 * aes->Nk] ^ temp[0];
        w[i + 1] = w[i + 1 - 4 * aes->Nk] ^ temp[1];
        w[i + 2] = w[i + 2 - 4 * aes->Nk] ^ temp[2];
        w[i + 3] = w[i + 3 - 4 * aes->Nk] ^ temp[3];
        i += 4;
    }
    free(rcon);
    free(temp);
}


void InvSubBytes(unsigned char **state, aes_base *aes) {
    int i, j;
    unsigned char t;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            t = state[i][j];
            state[i][j] = inv_sbox[t / 16][t % 16];
        }
    }
}


unsigned char mul_bytes(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char high_bit_mask = 0x80;
    unsigned char high_bit;
    unsigned char modulo = 0x1B;


    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }

        high_bit = a & high_bit_mask;
        a <<= 1;
        if (high_bit) {
            a ^= modulo;
        }
        b >>= 1;
    }

    return p;
}


void InvMixColumns(unsigned char **state, aes_base *aes) {
    unsigned char s[4], s1[4];
    int i, j;

    for (j = 0; j < aes->Nb; j++) {
        for (i = 0; i < 4; i++) {
            s[i] = state[i][j];
        }
        s1[0] = mul_bytes(0x0e, s[0]) ^ mul_bytes(0x0b, s[1]) ^ mul_bytes(0x0d, s[2]) ^ mul_bytes(0x09, s[3]);
        s1[1] = mul_bytes(0x09, s[0]) ^ mul_bytes(0x0e, s[1]) ^ mul_bytes(0x0b, s[2]) ^ mul_bytes(0x0d, s[3]);
        s1[2] = mul_bytes(0x0d, s[0]) ^ mul_bytes(0x09, s[1]) ^ mul_bytes(0x0e, s[2]) ^ mul_bytes(0x0b, s[3]);
        s1[3] = mul_bytes(0x0b, s[0]) ^ mul_bytes(0x0d, s[1]) ^ mul_bytes(0x09, s[2]) ^ mul_bytes(0x0e, s[3]);

        for (i = 0; i < 4; i++) {
            state[i][j] = s1[i];
        }
    }
}

void InvShiftRows(unsigned char **state, aes_base *aes) {
    ShiftRow(state, 1, aes->Nb - 1, aes);
    ShiftRow(state, 2, aes->Nb - 2, aes);
    ShiftRow(state, 3, aes->Nb - 3, aes);
}

void EncryptBlock(const unsigned char in[], unsigned char out[], unsigned char *roundKeys, aes_base *aes) {
    auto **state = (unsigned char **) malloc(sizeof(unsigned char *) * 4);
    state[0] = (unsigned char *) malloc(sizeof(unsigned char) * 4 * aes->Nb);
    int i, j, round;
    for (i = 0; i < 4; i++) {
        state[i] = state[0] + aes->Nb * i;
    }


    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            state[i][j] = in[i + 4 * j];
        }
    }

    AddRoundKey(state, roundKeys, aes);

    for (round = 1; round <= aes->Nr - 1; round++) {
        SubBytes(state, aes);
        ShiftRows(state, aes);
        MixColumns(state);
        AddRoundKey(state, roundKeys + round * 4 * aes->Nb, aes);
    }

    SubBytes(state, aes);
    ShiftRows(state, aes);
    AddRoundKey(state, roundKeys + aes->Nr * 4 * aes->Nb, aes);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            out[i + 4 * j] = state[i][j];
        }
    }
    free(state[0]);
    free(state);
}

void DecryptBlock(const unsigned char in[], unsigned char out[], unsigned char *roundKeys, aes_base *aes) {
    auto **state = (unsigned char **) malloc(sizeof(unsigned char *) * 4);
    state[0] = (unsigned char *) malloc(sizeof(unsigned char) * 4 * aes->Nb);
    int i, j, round;
    for (i = 0; i < 4; i++) {
        state[i] = state[0] + aes->Nb * i;
    }


    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            state[i][j] = in[i + 4 * j];
        }
    }

    AddRoundKey(state, roundKeys + aes->Nr * 4 * aes->Nb, aes);

    for (round = aes->Nr - 1; round >= 1; round--) {
        InvSubBytes(state, aes);
        InvShiftRows(state, aes);
        AddRoundKey(state, roundKeys + round * 4 * aes->Nb, aes);
        InvMixColumns(state, aes);
    }

    InvSubBytes(state, aes);
    InvShiftRows(state, aes);
    AddRoundKey(state, roundKeys, aes);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < aes->Nb; j++) {
            out[i + 4 * j] = state[i][j];
        }
    }
    free(state[0]);
    free(state);
}

aes_base *aes_init(int keyLen) {
    auto *aes = (aes_base *) malloc(sizeof(aes_base));
    aes->Nb = 4;
    switch (keyLen) {
        case 128:
            aes->Nk = 4;
            aes->Nr = 10;
            break;
        case 192:
            aes->Nk = 6;
            aes->Nr = 12;
            break;
        case 256:
            aes->Nk = 8;
            aes->Nr = 14;
            break;
        default:
            break;
    }
    aes->blockBytesLen = 4 * aes->Nb * sizeof(unsigned char);
    return aes;
}

aes_ctx Encrypt(const aes_ctx in, unsigned char key[], aes_base *aes) {
    aes_ctx out;
    out.length = GetPaddingLength(in.length, aes);
    out.ctx = (unsigned char *) malloc(sizeof(unsigned char) * out.length);
    unsigned char *alignIn = PaddingNulls(in.ctx, in.length, out.length);
    auto *roundKeys = (unsigned char *) malloc(4 * aes->Nb * (aes->Nr + 1) * sizeof(unsigned char));
    KeyExpansion(key, roundKeys, aes);
    for (unsigned i = 0; i < out.length; i += aes->blockBytesLen) {
        EncryptBlock(alignIn + i, out.ctx + i, roundKeys, aes);
    }
    free(alignIn);
    free(roundKeys);
    return out;
}

aes_ctx Decrypt(const aes_ctx in, unsigned char key[], aes_base *aes) {
    aes_ctx out;
    out.length = in.length;
    out.ctx = (unsigned char *) malloc(sizeof(unsigned char) * in.length);
    auto *roundKeys = (unsigned char *) malloc(4 * aes->Nb * (aes->Nr + 1) * sizeof(unsigned char));
    KeyExpansion(key, roundKeys, aes);
    for (unsigned i = 0; i < in.length; i += aes->blockBytesLen) {
        DecryptBlock(in.ctx + i, out.ctx + i, roundKeys, aes);
    }
    free(roundKeys);
    return out;
}

char *aesEncode(const char msg[], const size_t size, const char key[], size_t *res_size) {
    aes_base *aes = aes_init(256);
    aes_ctx _msg;
    _msg.length = 64;
    _msg.ctx = (unsigned char *) malloc(sizeof(unsigned char) * 64);
    for (int j = 0; j < 64; j++) _msg.ctx[j] = 0;

    unsigned times = size / 64 + (int) (size % 64 != 0);
    char *result = (char *) malloc((times * 64 + 4) * sizeof(char));
    {
        unsigned _temp = size, pos = 3;
        for(int i=0;i<4;i++) result[i]=0;
        while (_temp != 0) {
            result[pos] = (char) (_temp % 256);
            pos--;
            _temp >>= 8;
        }
    }

    for (unsigned i = 0; i < times; i++) {
        for (unsigned j = 0; j < 64 && i * 64 + j < size; j++) {
            _msg.ctx[j] = (unsigned char) msg[i * 64 + j];
        }
        aes_ctx out = Encrypt(_msg, (unsigned char *) key, aes);
        for (unsigned j = 0; j < out.length; j++) result[i * 64 + j + 4] = (char) out.ctx[j];
        free(out.ctx);
    }
    *res_size = (times * 64 + 4);
    free(_msg.ctx);
    return result;
}

char *aesDecode(const char plain_text[], const size_t size, const char key[], size_t *res_size) {
    aes_base *aes = aes_init(256);
    aes_ctx _msg;
    _msg.length = 64;
    _msg.ctx = (unsigned char *) malloc(sizeof(unsigned char) * 64);

    unsigned times, max_length = 0;
    for (int i = 0; i < 4; i++) {
        max_length <<= 8;
        max_length += (unsigned char) plain_text[i];
    }
    char *result = (char *) malloc(max_length * sizeof(char));
    times = size / 64;
    for (unsigned i = 0; i < times; i++) {
        for (unsigned j = 0; j < _msg.length; j++) {
            _msg.ctx[j] = (unsigned char) plain_text[i * 64 + j + 4];
        }

        aes_ctx out = Decrypt(_msg, (unsigned char *) key, aes);

        for (unsigned j = 0; j < out.length && i * 64 + j < max_length; j++) result[i * 64 + j] = (char) out.ctx[j];
        free(out.ctx);
    }
    free(_msg.ctx);
    *res_size = max_length;
    return result;
}

std::string crypto::aes::encode(const std::string &msg, const std::string &key) {
    size_t size = msg.length();
    size_t res_size = 0;

    char *c_res;


    c_res = aesEncode(msg.c_str(), size, key.c_str(), &res_size);

    std::string result;
    for (int i = 0; i < res_size; i++) {
        result += c_res[i];
    }
    free(c_res);
    return result;
}

std::string crypto::aes::decode(const std::string &plain_text, const std::string &key) {
    size_t size = plain_text.length();
    size_t res_size = 0;

    char *c_res;


    c_res = aesDecode(plain_text.c_str(), size, key.c_str(), &res_size);

    std::string result;
    for (int i = 0; i < res_size; i++) {
        result += c_res[i];
    }
    free(c_res);
    return result;
}