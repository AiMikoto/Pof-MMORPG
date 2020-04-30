#ifndef CLIENT_MANIPULATOR_H
#define CLIENT_MANIPULATOR_H

#include "lib/rcon.h"
#include <glm/glm.hpp>
#include "include/maps.h"
#include "scene/oid.h"

class manipulator:public rcon
{
public:
  manipulator(std::string hostname, int port, std::string tok);
  oid_t spawn();
  void set_slicer(bool mode);
  void set_sps(double sps);
  void obj_move(oid_t &id, glm::dvec3 pos);
  void obj_scale(oid_t &id, glm::dvec3 scale);
  void obj_rotate(oid_t &id, glm::dvec3 rotation);
  void obj_delete(oid_t &id);
  void comp_add(oid_t &target, boost::property_tree::ptree recipe);
  void obj_attach(oid_t &from, oid_t &to);
  void obj_rename(oid_t &target, std::string name);
  void obj_tag(oid_t &target, std::string tag);
  void obj_untag(oid_t &target, std::string tag);
  void save();
  void save(map_t map);
};

#endif // CLIENT_MANIPULATOR_H
