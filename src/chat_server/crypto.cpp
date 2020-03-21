#include <chat_server/crypto.h>
#include "lib/uuid.h"

rsa_crypto *g_rsa;

void init_crypto(std::string priv)
{
  BIO *keybio = BIO_new(BIO_s_file());
  BIO_read_filename(keybio, priv.c_str());
  g_rsa = new rsa_crypto(NULL, keybio);
}

void destroy_crypto()
{
  delete g_rsa;
}
