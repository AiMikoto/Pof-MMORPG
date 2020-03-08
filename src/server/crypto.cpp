#include <server/crypto.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

rsa_crypto *g_rsa;
aes_crypto *g_aes;

void init_crypto(std::string pub, std::string priv)
{
  boost::uuids::random_generator generator;
  std::string uuid1 = boost::lexical_cast<std::string>(generator());
  std::string uuid2 = boost::lexical_cast<std::string>(generator());
  std::string uuid3 = uuid1 + uuid2;
  std::string uuid4 = boost::lexical_cast<std::string>(generator());
  g_aes = new aes_crypto(uuid3.substr(0, 32), uuid4.substr(0, 16));
  BIO *keybio1 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio1, pub.c_str());
  BIO *keybio2 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio2, priv.c_str());
  g_rsa = new rsa_crypto(keybio1, keybio2);
}

