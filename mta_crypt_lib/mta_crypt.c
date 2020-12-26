#include "mta_crypt.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>

#define _GNU_SOURCE
#include <openssl/evp.h>

#define MTA_CRYPT_INPUT_KEY_VALIDATION(key, key_length)     \
    if (key == NULL){                                       \
        return MTA_CRYPT_RET_NULL_PTR_RECEIVED;             \
    }                                                       \
    if (key_length == 0){                                   \
        return MTA_CRYPT_RET_KEY_ZERO_LENGTH;               \
    }                                                       \
    if (key_length > EVP_MAX_KEY_LENGTH){                   \
        return MTA_CRYPT_RET_KEY_MAX_LENGTH_EXCEEDED;       \
    };

#define MTA_CRYPT_INPUT_DATA_VALIDATION(data, data_length)  \
    if (data == NULL){                                      \
        return MTA_CRYPT_RET_NULL_PTR_RECEIVED;             \
    }                                                       \
    if (data_length == 0){                                  \
        return MTA_CRYPT_RET_DATA_ZERO_LENGTH;              \
    }                                                       \
    if (data_length > EVP_MAX_KEY_LENGTH * 8){              \
        return MTA_CRYPT_RET_DATA_MAX_LENGTH_EXCEEDED;      \
    }                                                       \
    if (data_length % 8 != 0){                              \
        return MTA_CRYPT_RET_NOT_8_BYTE_MULTIPLICATION;     \
    };

#define MTA_CRYPT_NULL_VALIDATION(ptr)                      \
    if (ptr == NULL){                                       \
        return MTA_CRYPT_RET_NULL_PTR_RECEIVED;             \
    }

#define MTA_CRYPT_CHECK_EVP_RET(ret, action)                \
    if (ret != 1){                                          \
        action;                                             \
    }

MTA_CRYPT_RET_STATUS key_padding(char* key_in, char** key_out, unsigned int key_in_length);

MTA_CRYPT_RET_STATUS MTA_encrypt(char* key, unsigned int key_length, char* plain_data, unsigned int plain_data_length, char* encrypted_data, unsigned int* encrypted_data_length)
{
    int ret = MTA_CRYPT_RET_OK;
    EVP_CIPHER_CTX *ctx;
    int len;
    char* key_padded;
    int evp_ret = 1;

    MTA_CRYPT_NULL_VALIDATION(key);
    MTA_CRYPT_NULL_VALIDATION(plain_data);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data_length);

    MTA_CRYPT_INPUT_DATA_VALIDATION(plain_data, plain_data_length);

    ret = key_padding(key, &key_padded, key_length);
    if (ret != MTA_CRYPT_RET_OK) return ret;

    if(!(ctx = EVP_CIPHER_CTX_new())){
        ret = MTA_CRYPT_RET_ERROR;
        goto fin;
    }

    MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptUpdate(ctx, encrypted_data, &len, plain_data, plain_data_length), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *encrypted_data_length = len;

    // Finalize the encryption. Further ciphertext bytes may be written at this stage
    MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptFinal_ex(ctx, encrypted_data + len, &len), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *encrypted_data_length += (unsigned int)len;

fin:
    EVP_CIPHER_CTX_free(ctx);
    free(key_padded);

    return ret;
}

MTA_CRYPT_RET_STATUS MTA_decrypt(char* key, unsigned int key_length, char* encrypted_data, unsigned int encrypted_data_length, char* plain_data, unsigned int* plain_data_length)
{
    MTA_CRYPT_RET_STATUS ret = MTA_CRYPT_RET_OK;
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    char* key_padded;
    int evp_ret = 1;

    MTA_CRYPT_NULL_VALIDATION(key);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data);
    MTA_CRYPT_NULL_VALIDATION(plain_data);
    MTA_CRYPT_NULL_VALIDATION(plain_data_length);

    ret = key_padding(key, &key_padded, key_length);
    if (ret != MTA_CRYPT_RET_OK) return ret;

    if(!(ctx = EVP_CIPHER_CTX_new())){
        ret = MTA_CRYPT_RET_ERROR;
        goto fin;
    }

    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_CIPHER_CTX_set_padding(ctx, 0), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptUpdate(ctx, plain_data, &len, encrypted_data, encrypted_data_length), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *plain_data_length = (unsigned int)len;

    // Finalize the encryption. Further ciphertext bytes may be written at this stage
    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptFinal_ex(ctx, plain_data + len, &len), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *plain_data_length += (unsigned int)len;

fin:
    EVP_CIPHER_CTX_free(ctx);
    free(key_padded);

    return ret;
}

MTA_CRYPT_RET_STATUS key_padding(char* key_in, char** key_out, unsigned int key_in_length)
{
    MTA_CRYPT_INPUT_KEY_VALIDATION(key_in, key_in_length);
    
    *key_out = (char*)malloc(EVP_MAX_KEY_LENGTH);
    bzero(*key_out, EVP_MAX_KEY_LENGTH);
    memcpy(*key_out, key_in, key_in_length);

    return MTA_CRYPT_RET_OK;
}
