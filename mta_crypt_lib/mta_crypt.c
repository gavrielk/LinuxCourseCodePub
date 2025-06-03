#include "mta_crypt.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/provider.h>

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

#define MTA_CRYPT_INIT_VALIDATION(provider)                 \
    if (provider == NULL){                                  \
        return MTA_CRYPT_RET_INIT_NOT_CALLED;               \
    }

#define MTA_CRYPT_CHECK_EVP_RET(ret, action)                \
    if (ret != 1){                                          \
        fprintf(stderr, "%d: OpenSSL error: %s\n", __LINE__, ERR_reason_error_string(ERR_get_error())); \
        action;                                             \
    }

const OSSL_PROVIDER *provider = NULL;
const EVP_CIPHER *cipher = NULL;

MTA_CRYPT_RET_STATUS MTA_crypt_init(){
    provider = OSSL_PROVIDER_load(NULL, "legacy");
    if (provider == NULL) {
        return MTA_CRYPT_RET_ERROR;
    }
    
    cipher = EVP_get_cipherbyname("RC2-ECB");
    if (cipher == NULL) {
        return MTA_CRYPT_RET_ERROR;
    }

    return MTA_CRYPT_RET_OK;
}

MTA_CRYPT_RET_STATUS MTA_encrypt(char* key, unsigned int key_length, char* plain_data, unsigned int plain_data_length, char* encrypted_data, unsigned int* encrypted_data_length)
{
    int ret = MTA_CRYPT_RET_OK;
    EVP_CIPHER_CTX *ctx = 0;
    int len = 0;

    MTA_CRYPT_INPUT_KEY_VALIDATION(key, key_length);
    MTA_CRYPT_NULL_VALIDATION(plain_data);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data_length);
    MTA_CRYPT_INIT_VALIDATION(provider);
    MTA_CRYPT_INIT_VALIDATION(cipher);

    MTA_CRYPT_INPUT_DATA_VALIDATION(plain_data, plain_data_length);

    if(!(ctx = EVP_CIPHER_CTX_new())){
        ret = MTA_CRYPT_RET_ERROR;
        goto fin;
    }

    MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptInit_ex(ctx, cipher, NULL, NULL, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_CIPHER_CTX_set_key_length(ctx, key_length), ret = MTA_CRYPT_RET_ERROR; goto fin);
    
	MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptInit_ex(ctx, NULL, NULL, key, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_CIPHER_CTX_set_padding(ctx, 0), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_EncryptUpdate(ctx, encrypted_data, &len, plain_data, plain_data_length), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *encrypted_data_length = len;

fin:
    EVP_CIPHER_CTX_free(ctx);

    return ret;
}

MTA_CRYPT_RET_STATUS MTA_decrypt(char* key, unsigned int key_length, char* encrypted_data, unsigned int encrypted_data_length, char* plain_data, unsigned int* plain_data_length)
{
    MTA_CRYPT_RET_STATUS ret = MTA_CRYPT_RET_OK;
    EVP_CIPHER_CTX *ctx = 0;
    int len = 0;

    MTA_CRYPT_INPUT_KEY_VALIDATION(key, key_length);
    MTA_CRYPT_NULL_VALIDATION(encrypted_data);
    MTA_CRYPT_NULL_VALIDATION(plain_data);
    MTA_CRYPT_NULL_VALIDATION(plain_data_length);
    MTA_CRYPT_INIT_VALIDATION(provider);
    MTA_CRYPT_INIT_VALIDATION(cipher);

    if(!(ctx = EVP_CIPHER_CTX_new())){
        ret = MTA_CRYPT_RET_ERROR;
        goto fin;
    }
	
    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptInit_ex(ctx, cipher, NULL, NULL, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_CIPHER_CTX_set_key_length(ctx, key_length), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptInit_ex(ctx, NULL, NULL, key, NULL), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_CIPHER_CTX_set_padding(ctx, 0), ret = MTA_CRYPT_RET_ERROR; goto fin);

    MTA_CRYPT_CHECK_EVP_RET(EVP_DecryptUpdate(ctx, plain_data, &len, encrypted_data, encrypted_data_length), ret = MTA_CRYPT_RET_ERROR; goto fin);
    *plain_data_length = (unsigned int)len;

fin:
    EVP_CIPHER_CTX_free(ctx);

    return ret;
}
