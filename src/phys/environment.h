#ifndef PHYS_ENVIRONMENT_H
#define PHYS_ENVIRONMENT_H

#include "phys/capsule.h"
#include <map>

class environment
{
public:
  environment();
  ~environment();
  int add(capsule *c);
  std::map<int, capsule *> capsules;
  int counter;
};

#endif // PHYS_ENVIRONMENT_H
