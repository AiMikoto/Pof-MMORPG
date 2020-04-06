#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <cstdio>
#include <iostream>
#include "include/common_macro.h"
#include "lib/log.h"

int main()
{
  log_init("manipulator");
  log_mute(); // we want silence for reading purposes
  init_crypto("keys/public_key.pem");
  manipulator man("localhost", 7000, "fish");
  forever
  {
    std::string line;
    getline(std::cin, line);
    if(line == "set slicer off")
    {
      man.set_slicer(false);
      continue;
    }
    if(line == "set slicer on")
    {
      man.set_slicer(true);
      continue;
    }
    printf("unknown command\n");
  }
  return 0;
  destroy_crypto();
}
