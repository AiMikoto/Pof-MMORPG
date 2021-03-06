#include "manipulator/crypto.h"
#include "manipulator/manipulator.h"
#include <cstdio>
#include <iostream>
#include "include/common_macro.h"
#include "lib/log.h"
#ifdef __linux__
#include <readline/readline.h>
#include <readline/history.h>
#endif

manipulator *man;

bool try_exit(std::string line)
{
  return line == "exit" || line == "quit";
}

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

bool try_scale(std::string line)
{
  unsigned long long id;
  glm::dvec3 scale;
  if(sscanf(line.c_str(), "scale %llu {%lf, %lf, %lf}", &id, &scale.x, &scale.y, &scale.z) == 4)
  {
    man -> obj_scale(id, scale);
    return true;
  }
  return false;
}

bool try_rotate(std::string line)
{
  unsigned long long id;
  glm::dvec3 rotation;
  if(sscanf(line.c_str(), "rotate %llu {%lf, %lf, %lf}", &id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    man -> obj_rotate(id, rotation);
    return true;
  }
  if(sscanf(line.c_str(), "rotate %llu rad {%lf, %lf, %lf}", &id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    man -> obj_rotate(id, rotation);
    return true;
  }
  if(sscanf(line.c_str(), "rotate %llu deg {%lf, %lf, %lf}", &id, &rotation.x, &rotation.y, &rotation.z) == 4)
  {
    rotation.x = rotation.x * M_PI / 180;
    rotation.y = rotation.y * M_PI / 180;
    rotation.z = rotation.z * M_PI / 180;
    man -> obj_rotate(id, rotation);
    return true;
  }
  return false;
}

bool try_save(std::string line)
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

bool try_spawn(std::string line)
{
  if(line == "spawn")
  {
    printf("spawned [%llu]\n", man -> spawn());
    return true;
  }
  return false;
}

bool try_delete(std::string line)
{
  unsigned long long id;
  if(sscanf(line.c_str(), "delete %llu", &id) == 1)
  {
    man -> obj_delete(id);
    return true;
  }
  return false;
}

bool try_add(std::string line)
{
  unsigned long long id;
  boost::property_tree::ptree recipe;
  if(sscanf(line.c_str(), "add solid_object to %llu", &id) == 1)
  {
    recipe.put("type", "solid_object");
    man -> comp_add(id, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider to %llu", &id) == 1)
  {
    recipe.put("type", "physical_collider");
    recipe.put("shape", "sphere");
    man -> comp_add(id, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider box to %llu", &id) == 1)
  {
    recipe.put("type", "physical_collider");
    recipe.put("shape", "box");
    man -> comp_add(id, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add phys_collider caps to %llu", &id) == 1)
  {
    recipe.put("type", "physical_collider");
    recipe.put("shape", "caps");
    man -> comp_add(id, recipe);
    return true;
  }
  char path[256];
  if(sscanf(line.c_str(), "add mesh_filter %s to %llu", path, &id) == 2)
  {
    recipe.put("type", "mesh_filter");
    recipe.put("path", std::string(path));
    man -> comp_add(id, recipe);
    return true;
  }
  if(sscanf(line.c_str(), "add mesh_renderer to %llu", &id) == 1)
  {
    recipe.put("type", "mesh_renderer");
    man -> comp_add(id, recipe);
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
#ifdef __linux__
    char *l = readline("> ");
    if(!l)
    {
      break;
    }
    add_history(l);
    line = std::string(l);
    free(l);
#else
    getline(std::cin, line);
#endif
    if(try_exit(line))
    {
      break;
    }
    try_rotate(line) || try_scale(line) || try_add(line) || try_delete(line) || try_spawn(line) || try_save(line) || try_ss(line) || try_sps(line) || try_move(line) || printf("unknown command\n");
  }
  return 0;
  destroy_crypto();
  delete man;
}
