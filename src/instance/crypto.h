#ifndef INSTANCE_CRYPTO_H
#define INSTANCE_CRYPTO_H

#include "lib/crypto.h"

extern rsa_crypto *g_rsa;
extern aes_crypto *g_aes;

void init_crypto(std::string pub, std::string priv);

#endif
