#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include "rsa_utils.h"

RSA* create_rsa_keypair() {
    RSA* rsa = RSA_new();
    BIGNUM* bne = BN_new();
    BN_set_word(bne, RSA_F4);
    RSA_generate_key_ex(rsa, 2048, bne, NULL);
    BN_free(bne);
    return rsa;
}

int rsa_encrypt(RSA* rsa_pub, unsigned char* message, unsigned char* encrypted) {
    return RSA_public_encrypt(strlen((char*)message) + 1, message, encrypted, rsa_pub, RSA_PKCS1_PADDING);
}

int rsa_decrypt(RSA* rsa_priv, unsigned char* encrypted, unsigned char* decrypted) {
    return RSA_private_decrypt(RSA_size(rsa_priv), encrypted, decrypted, rsa_priv, RSA_PKCS1_PADDING);
}
