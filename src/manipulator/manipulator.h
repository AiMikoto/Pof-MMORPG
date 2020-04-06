#ifndef CLIENT_MANIPULATOR_H
#define CLIENT_MANIPULATOR_H

#include "lib/rcon.h"

class manipulator:public rcon
{
public:
  manipulator(std::string hostname, int port, std::string tok);
  unsigned long long spawn();
  void set_slicer(bool mode);
};

#endif // CLIENT_MANIPULATOR_H
