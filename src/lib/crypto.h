#ifndef LIB_CRYPTO_H
#define LIB_CRYPTO_H

#include <string>
#include <openssl/pem.h>

class crypto
{
public:
  virtual std::string encrypt(std::string data) = 0;
  virtual std::string decrypt(std::string data) = 0;
};

class aes_crypto:public crypto
{
public:
  aes_crypto(std::string key, std::string iv);
  std::string encrypt(std::string data);
  std::string decrypt(std::string data);
  int block_size = 128;
  std::string key;
  std::string iv;
};

class rsa_crypto:public crypto
{
public:
  rsa_crypto(BIO *enc, BIO *dec);
  ~rsa_crypto();
  std::string encrypt(std::string data);
  std::string decrypt(std::string data);
  BIO *enc;
  BIO *dec;
  int padding_convention = RSA_PKCS1_OAEP_PADDING;
};

#endif // LIB_CRYPTO_H
