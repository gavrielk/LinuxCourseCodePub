/*
 * MTA encryption utility library providing encryption and decryption over RC2-ECB encryption algorithm
 * The main motivation is to simplify the standard Openssl APIs 
 * Note 1: Caller should link with libcrypto(-lcrypto)
 * Note 2: Data length must be a multiplication of 8(MTA_CRYPT_RET_NOT_8_BYTE_MULTIPLICATION returned if not)
 * Author: Gabi Karasin
 */

typedef enum {
    MTA_CRYPT_RET_OK,
    MTA_CRYPT_RET_ERROR,
    MTA_CRYPT_RET_INIT_NOT_CALLED,
    MTA_CRYPT_RET_NULL_PTR_RECEIVED,
    MTA_CRYPT_RET_DATA_ZERO_LENGTH,
    MTA_CRYPT_RET_DATA_MAX_LENGTH_EXCEEDED,
    MTA_CRYPT_RET_KEY_ZERO_LENGTH,
    MTA_CRYPT_RET_KEY_MAX_LENGTH_EXCEEDED,
    MTA_CRYPT_RET_NOT_8_BYTE_MULTIPLICATION,
} MTA_CRYPT_RET_STATUS;

/*
    * Function:    MTA_crypt_init
    * Description: Initialize the MTA crypt library, load the legacy provider and set the cipher to RC2-ECB
    * --------------------------------------------------------------------------------------------
    * Returns:    MTA_CRYPT_RET_OK on success, or an error code on failure
    * Note:       This function *MUST* be called before any encryption or decryption operations
*/
MTA_CRYPT_RET_STATUS MTA_crypt_init();

/*
 * Function:    MTA_encrypt
 * Description: Encrypt data using ECB encryption algorithm
 * --------------------------------------------------------------------------------------------
 * [in]     key                     - the key that the data will be encrypted by
 * [in]     key_length              - length in bytes of the key variable
 * [in]     plain_data              - pointer to a buffer which holds plain data that will be encryted
 * [in]     plain_data_length       - length in bytes of the plain data buffer
 * [out]    encrypted_data          - pointer to the encrypted data buffer
 * [out]    encrypted_data_length   - length in bytes of the encrypted data buffer
 * encrypted_data_length might be bigger then plain_data_length
 */
MTA_CRYPT_RET_STATUS MTA_encrypt(char* key, unsigned int key_length, char* plain_data, unsigned int plain_data_length, char* encrypted_data, unsigned int* encrypted_data_length);

/*
 * Function:    MTA_decrypt
 * Description: Decrypt data using ECB encryption algorithm
 * --------------------------------------------------------------------------------------------
 * [in]     key                     - the key that the data will be encrypted by
 * [in]     key_length              - length in bytes of the key variable
 * [in]     encrypted_data          - pointer to an encrypted buffer that will be decrypted
 * [in]     encrypted_data_length   - length in bytes of the encrypted data buffer
 * [out]    plain_data              - pointer to the decrypted data
 * [out]    plain_data_length       - length in bytes of the plain data buffer
 */
MTA_CRYPT_RET_STATUS MTA_decrypt(char* key, unsigned int key_length, char* encrypted_data, unsigned int encrypted_data_length, char* plain_data, unsigned int* plain_data_length);
