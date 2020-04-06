#ifndef CLIENT_MANIPULATOR_H
#define CLIENT_MANIPULATOR_H

#include "lib/rcon.h"
#include <glm/glm.hpp>
#include "include/maps.h"

class manipulator:public rcon
{
public:
  manipulator(std::string hostname, int port, std::string tok);
  unsigned long long spawn();
  void set_slicer(bool mode);
  void set_sps(double sps);
  void obj_move(unsigned long long id, glm::dvec3 pos);
  void save();
  void save(map_t map);
};

#endif // CLIENT_MANIPULATOR_H
