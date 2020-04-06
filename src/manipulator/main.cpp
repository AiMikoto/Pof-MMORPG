#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <cstdio>
#include <iostream>
#include "include/common_macro.h"

int main()
{
  init_crypto("keys/public_key.pem");
  manipulator man("localhost", 7000, "fish");
  forever
  {
  }
  return 0;
  destroy_crypto();
}
