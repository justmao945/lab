
#ifndef CRYPT_H_
#define CRYPT_H_

#include<unistd.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/des.h>  // for DES
#include<openssl/rsa.h>  // for RSA
#include<openssl/rand.h>
#include<openssl/pem.h>
#include<openssl/md5.h>



/// DES encrypt/decrypt.
/// @param in  Input data.
/// @param len Length of input.
/// @param op  DES_ENCRYPT or DES_DECRYPT
/// @param key DES key.
/// @param rlen Returned length of data.
/// @return An new encypt/decrypt string, or NULL if failed.
char* des_crypt(const char *in, int len, int op, DES_cblock key, int *rlen);


/// add RSA sign first and encrypt data.
/// !! Returned data:
///    ..... 128? RSA_size(rsa) sig....
///    ..... DES encription....
/// @param data Orignial data.
/// @param len Length of original data.
/// @param rsa RSA object to sign.
/// @param des_key DES key to encrypt.
/// @param rlen  Returned string length.
/// @return An new encrypted string or  NULL if error
char *encrypt(const char *data, int len, RSA *rsa, DES_cblock des_key,
    int *rlen);



/// Decrypt RSA sign first and decrypt data.
/// !! input data:
///    ..... 128? RSA_size(rsa) sig....
///    ..... DES encription....
/// @param data Orignial data.
/// @param len Length of original data.
/// @param rsa RSA object to verify.
/// @param des_key DES key to decrypt.
/// @param rlen  Returned string length.
/// @return An new decrypted string or  NULL if error
char *decrypt(const char *data, int len, RSA *rsa, DES_cblock des_key,
    int *rlen);



/// Create new crypt files.
/// @param des_key Returned new DES key.
/// @param xxx_xxx_fname File names.
/// @return New RSA object or NULL.
RSA* crypt_new( DES_cblock des_key,
    const char *des_key_fname,
    const char *rsa_pub_fname,
    const char *rsa_priv_fname );


/// Read exist crypt files.(only des key and public key)
/// @param des_key Returned new DES key.
/// @param xxx_xxx_fname File names.
/// @return New RSA object or NULL.
RSA* crypt_read( DES_cblock des_key,
    const char *des_key_fname,
    const char *rsa_pub_fname );


/// A simple test.
void test_crypt( RSA *rsa, RSA *rsa_pub,
    DES_cblock des_key, const char *data, int len );

#endif

