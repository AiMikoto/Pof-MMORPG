#include "lib/crypto.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;
std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. fin";

void test_aes_encryption_decryption()
{
  TEST("TESTING AES CRYPTO");
  crypto *c = new aes_crypto("01234567890123456789012345678901", "0123456789012345");
  std::string enc = c -> encrypt(text);
  std::string dec = c -> decrypt(enc);
  if(text == dec)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  delete c;
}

void test_rsa_encryption_decryption()
{
  TEST("TESTING RSA CRYPTO");
  int rsa_size = 2048;
  RSA *keypair = RSA_generate_key(rsa_size, 3, NULL, NULL);
  BIO *dec_key = BIO_new(BIO_s_mem());
  BIO *enc_key = BIO_new(BIO_s_mem());
  PEM_write_bio_RSAPrivateKey(dec_key, keypair, NULL, NULL, 0, NULL, NULL);
  PEM_write_bio_RSAPublicKey(enc_key, keypair);
  crypto *c = new rsa_crypto(enc_key, dec_key);
  std::string text = "chocoturtle";
  std::string enc = c -> encrypt(text);
  std::string dec = c -> decrypt(enc);
  if(text == dec)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  delete c;
  RSA_free(keypair);
}

void test_BIG_rsa_encryption_decryption()
{
  TEST("TESTING BIG RSA CRYPTO");
  int rsa_size = 8192;
  BIGNUM *bne = BN_new();
  BN_set_word(bne, 3);
  RSA *keypair = RSA_new();
  RSA_generate_key_ex(keypair, rsa_size, bne, NULL);
  BIO *dec_key = BIO_new(BIO_s_mem());
  BIO *enc_key = BIO_new(BIO_s_mem());
  PEM_write_bio_RSAPrivateKey(dec_key, keypair, NULL, NULL, 0, NULL, NULL);
  PEM_write_bio_RSAPublicKey(enc_key, keypair);
  crypto *c = new rsa_crypto(enc_key, dec_key);
  std::string text = "soft cuddly teddy bears wearing pink jackets";
  std::string enc = c -> encrypt(text);
  std::string dec = c -> decrypt(enc);
  if(text == dec)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  delete c;
  RSA_free(keypair);
  BN_free(bne);
}

int main()
{
  test_aes_encryption_decryption();
//  test_rsa_encryption_decryption();
  test_BIG_rsa_encryption_decryption();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}
