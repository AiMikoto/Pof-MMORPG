#ifndef CHAT_SERVER_CRYPTO_H
#define CHAT_SERVER_CRYPTO_H

#include "lib/crypto.h"

extern rsa_crypto *g_rsa;

void init_crypto(std::string priv);

void destroy_crypto();

#endif // CHAT_SERVER_CRYPTO_H
