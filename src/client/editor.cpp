#include "client/editor.h"
#include "client/game.h"
#include <boost/format.hpp>

#define GET_OID(oid, x) try {oid = oid_t(std::string(x));} catch(std::exception &e) {csm -> say("unparsable id " + std::string(x)); return true;}

bool editor::try_ss(std::string line)
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

bool editor::try_sps(std::string line)
{
  double sps;
  if(sscanf(line.c_str(), "set sps %lf", &sps) == 1)
  {
    man -> set_sps(sps);
    return true;
  }
  return false;
}

bool editor::try_move(std::string line)
{
  glm::dvec3 pos;
  char id[256];
  if(sscanf(line.c_str(), "move %s {%lf, %lf, %lf}", id, &pos.x, &pos.y, &pos.z) == 4)
  {
    oid_t oid;
    GET_OID(oid, id);
    man -> obj_move(oid, pos);
    return true;
  }
  return false;
}

bool editor::try_scale(std::string line)
{
  glm::dvec3 scale;
  char id[256];
  if(sscanf(line.c_str(), "scale %s {%lf, %lf, %lf}", id, &scale.x, &scale.y, &scale.z) == 4)
  {
    oid_t oid;
    GET_OID(oid, id);
    man -> obj_scale(oid, scale);
    return true;
  }
  return false;
}

bool editor::try_rotate(std::string line)
{
  glm::dvec3 rotation;
  char id[256];
  if(sscanf(line.c_str(), "rotate %s {%lf, %lf, %lf}", id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    oid_t oid;
    GET_OID(oid, id);
    man -> obj_rotate(oid, rotation);
    return true;
  }
  if(sscanf(line.c_str(), "rotate %s rad {%lf, %lf, %lf}", id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    oid_t oid;
    GET_OID(oid, id);
    man -> obj_rotate(oid, rotation);
    return true;
  }
  if(sscanf(line.c_str(), "rotate %s deg {%lf, %lf, %lf}", id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    oid_t oid;
    GET_OID(oid, id);
    rotation.x = rotation.x * M_PI / 180;
    rotation.y = rotation.y * M_PI / 180;
    rotation.z = rotation.z * M_PI / 180;
    man -> obj_rotate(oid, rotation);
    return true;
  }
  return false;
}

bool editor::try_save(std::string line)
{
  if(line == "save")
  {
    man -> save();
    return true;
  }
  char file[256];
  if(sscanf(line.c_str(), "save %s", file) == 1)
  {
    man -> save(std::string(file));
    return true;
  }
  return false;
}

bool editor::try_spawn(std::string line)
{
  if(line == "spawn")
  {
    csm -> say("spawned [" + man -> spawn().serialise() + "]");
    return true;
  }
  return false;
}

bool editor::try_delete(std::string line)
{
  char id[256];
  if(sscanf(line.c_str(), "delete %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    man -> obj_delete(oid);
    return true;
  }
  return false;
}

bool editor::try_add(std::string line)
{
  boost::property_tree::ptree recipe;
  char id[256];
  if(sscanf(line.c_str(), "add solid_object to %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "solid_object");
    man -> comp_add(oid, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider sphere to %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "physical_collider");
    recipe.put("shape", "sphere");
    man -> comp_add(oid, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider box to %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "physical_collider");
    recipe.put("shape", "box");
    man -> comp_add(oid, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider caps to %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "physical_collider");
    recipe.put("shape", "caps");
    man -> comp_add(oid, recipe);
    return true;
  }
  char path[256];
  if(sscanf(line.c_str(), "add mesh_filter %s to %s", path, id) == 2)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "mesh_filter");
    recipe.put("path", std::string(path));
    man -> comp_add(oid, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add mesh_renderer to %s", id) == 1)
  {
    oid_t oid;
    GET_OID(oid, id);
    recipe.put("type", "mesh_renderer");
    man -> comp_add(oid, recipe);
    return true;
  }
  return false;
}

bool editor::try_attach(std::string line)
{
  char from[256], to[256];
  if(sscanf(line.c_str(), "attach %s to %s", from, to) == 2)
  {
    oid_t oid_from, oid_to;
    GET_OID(oid_from, from);
    GET_OID(oid_to, to);
    man -> obj_attach(oid_from, oid_to);
    return true;
  }
  return false;
}

editor::editor(std::string hostname, int port, std::string tok)
{
  man = new manipulator(hostname, port, tok);
}

editor::~editor()
{
  delete man;
}

bool editor::try_handle(std::string line)
{
  return try_rotate(line) || try_scale(line) || try_add(line) || try_delete(line) || try_spawn(line) || try_save(line) || try_ss(line) || try_sps(line) || try_move(line) || try_attach(line);
}
