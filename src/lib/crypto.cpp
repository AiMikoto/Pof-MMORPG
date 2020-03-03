#include "lib/crypto.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <iostream>

aes_crypto::aes_crypto(std::string key, std::string iv)
{
  if((key.size() != 32) || (iv.size() != 16))
  {
    throw std::logic_error("incorrect cypher sizes");
  }
  this -> key = key;
  this -> iv = iv;
}

std::string aes_crypto::encrypt(std::string data)
{
  uint32_t size = data.length();
  // allocate buffer
  unsigned char buffer[size * 2];
  // initialise encryption operation
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());
  int len;
  // encrypt
  EVP_EncryptUpdate(ctx, buffer, &len, (unsigned char *)data.c_str(), size);
  uint32_t output_size = len;
  EVP_EncryptFinal_ex(ctx, buffer + len, &len);
  output_size += len;
  // clean-up
  EVP_CIPHER_CTX_free(ctx);
  return std::string((char *)buffer, output_size);
}

std::string aes_crypto::decrypt(std::string data)
{
  uint32_t size = data.length();
  // allocate buffer
  unsigned char buffer[size * 2];
  // initialise decryption operation
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());
  int len;
  // decrypt
  EVP_DecryptUpdate(ctx, buffer, &len, (unsigned char *)data.c_str(), size);
  uint32_t output_size = len;
  EVP_DecryptFinal_ex(ctx, buffer + len, &len);
  output_size += len;
  // clean-up
  EVP_CIPHER_CTX_free(ctx);
  return std::string((char *)buffer, output_size);
}

rsa_crypto::rsa_crypto(BIO *enc, BIO *dec)
{
  key = RSA_new();
  this -> enc = enc;
  this -> dec = dec;
  // initialise encryption operation
  if(enc)
  {
    if(PEM_read_bio_RSA_PUBKEY(enc, &key, NULL, NULL) == NULL)
    {
      throw std::logic_error("invalid public key");
    }
  }
  if(dec)
  {
    if(PEM_read_bio_RSAPrivateKey(dec, &key, NULL, NULL) == NULL)
    {
      throw std::logic_error("invalid private key");
    }
  }
}

rsa_crypto::~rsa_crypto()
{
  if(enc)
  {
    BIO_free_all(enc);
  }
  if(dec)
  {
    BIO_free_all(dec);
  }
  RSA_free(key);
}

std::string rsa_crypto::encrypt(std::string data)
{
  uint32_t size = data.length();
  // allocate buffer
  unsigned char buffer[RSA_size(key)];
  // encrypt
  uint32_t output_size = RSA_public_encrypt(size, (unsigned char *)data.c_str(), buffer, key, padding_convention);
  // clean-up
  return std::string((char *)buffer, output_size);
}

std::string rsa_crypto::decrypt(std::string data)
{
  uint32_t size = data.length();
  // allocate buffer
  unsigned char buffer[RSA_size(key)];
  // decrypt
  uint32_t output_size = RSA_private_decrypt(size, (unsigned char *)data.c_str(), buffer, key, padding_convention);
  // clean-up
  return std::string((char *)buffer, output_size);
}
