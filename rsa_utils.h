#ifndef RSA_UTILS_H
#define RSA_UTILS_H

#include <openssl/rsa.h>

RSA* create_rsa_keypair();
int rsa_encrypt(RSA* rsa_pub, unsigned char* message, unsigned char* encrypted);
int rsa_decrypt(RSA* rsa_priv, unsigned char* encrypted, unsigned char* decrypted);

#endif
