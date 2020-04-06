#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <cstdio>
#include <iostream>
#include "include/common_macro.h"
#include "lib/log.h"

manipulator *man;

bool try_ss(std::string line)
{
  if(line == "set slicer on")
  {
    man -> set_slicer(true);
    return true;
  }
  if(line == "set slicer off")
  {
    man -> set_slicer(false);
    return true;
  }
  return false;
}

bool try_sps(std::string line)
{
  double sps;
  if(sscanf(line.c_str(), "set sps %lf", &sps) == 1)
  {
    man -> set_sps(sps);
    return true;
  }
  return false;
}

bool try_move(std::string line)
{
  unsigned long long id;
  glm::dvec3 pos;
  if(sscanf(line.c_str(), "move %llu {%lf, %lf, %lf}", &id, &pos.x, &pos.y, &pos.z) == 4)
  {
    man -> obj_move(id, pos);
    return true;
  }
  return false;
}

int main()
{
  log_init("manipulator");
  init_crypto("keys/public_key.pem");
  man = new manipulator("localhost", 7000, "fish");
  log_mute(); // we want silence for reading purposes
  forever
  {
    std::string line;
    getline(std::cin, line);
    try_ss(line) || try_sps(line) || try_move(line) || printf("unknown command\n");
  }
  return 0;
  destroy_crypto();
  delete man;
}
