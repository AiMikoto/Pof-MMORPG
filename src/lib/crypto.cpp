#include "lib/crypto.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>

std::string crypto::encrypt(std::string data)
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

std::string crypto::decrypt(std::string data)
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
