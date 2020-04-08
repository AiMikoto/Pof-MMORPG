#ifndef CLIENT_CRYPTO_H
#define CLIENT_CRYPTO_H

#include "lib/crypto.h"

extern rsa_crypto *g_rsa;
extern aes_crypto *g_aes;

void init_crypto(std::string pub);

void destroy_crypto();

#endif // CLIENT_CRYPTO_H
