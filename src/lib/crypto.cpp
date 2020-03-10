#include "lib/crypto.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string sha256(std::string input)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha;
  SHA256_Init(&sha);
  SHA256_Update(&sha, input.c_str(), input.size());
  SHA256_Final(hash, &sha);
  std::stringstream ss;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return ss.str();
}

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
  const uint32_t buffer_size = size * 2;
  unsigned char buffer[buffer_size];
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
  const uint32_t buffer_size = size * 2;
  unsigned char buffer[buffer_size];
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
  const uint32_t buffer_size = RSA_size(key);
  unsigned char buffer[buffer_size];
  // encrypt
  uint32_t output_size = RSA_public_encrypt(size, (unsigned char *)data.c_str(), buffer, key, padding_convention);
  // clean-up
  return std::string((char *)buffer, output_size);
}

std::string rsa_crypto::decrypt(std::string data)
{
  uint32_t size = data.length();
  // allocate buffer
  const uint32_t buffer_size = RSA_size(key);
  unsigned char buffer[buffer_size];
  // decrypt
  uint32_t output_size = RSA_private_decrypt(size, (unsigned char *)data.c_str(), buffer, key, padding_convention);
  // clean-up
  return std::string((char *)buffer, output_size);
}
