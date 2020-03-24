#include <instance/crypto.h>
#include "lib/uuid.h"

rsa_crypto *g_rsa;
aes_crypto *g_aes;

void init_crypto(std::string pub, std::string priv)
{
  std::string uuid1 = get_uuid();
  std::string uuid2 = get_uuid();
  std::string uuid3 = uuid1 + uuid2;
  std::string uuid4 = get_uuid();
  g_aes = new aes_crypto(uuid3.substr(0, 32), uuid4.substr(0, 16));
  BIO *keybio1 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio1, pub.c_str());
  BIO *keybio2 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio2, priv.c_str());
  g_rsa = new rsa_crypto(keybio1, keybio2);
}

void destroy_crypto()
{
  delete g_rsa;
  delete g_aes;
}
