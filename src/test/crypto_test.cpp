#include "lib/crypto.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_encryption_decryption()
{
  TEST("TESTING CRYPTO");
  std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tristique, libero a efficitur sagittis, ipsum massa aliquet felis, vitae elementum tellus est id ipsum. Suspendisse dapibus tempus egestas. Nam lacinia mauris massa, quis rhoncus velit efficitur sit amet. Pellentesque a orci commodo, rhoncus ligula sed, rhoncus tortor. Vestibulum ultrices risus ac odio sagittis, id eleifend felis imperdiet. Phasellus vel odio odio. Aliquam eu volutpat magna, in auctor massa. fin";
  crypto c;
  c.key = "01234567890123456789012345678901";
  c.iv = "0123456789012345";
  std::string enc = c.encrypt(text);
  std::string dec = c.decrypt(enc);
  if(text == dec)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
}

int main()
{
  test_encryption_decryption();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}
