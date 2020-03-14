#ifndef PHYS_ENVIRONMENT_H
#define PHYS_ENVIRONMENT_H

#include "phys/container.h"
#include <map>

class environment
{
public:
  environment();
  ~environment();
  int add(container *c);
  std::map<int, container *> containers;
  int counter;
};

#endif // PHYS_ENVIRONMENT_H
