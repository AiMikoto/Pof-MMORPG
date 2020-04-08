#ifndef LIB_DATABASE_H
#define LIB_DATABASE_H

#include <string>
#include "common/user_card.h"
#include <pqxx/pqxx>
#include "scene/scene.h"
#include "include/maps.h"
#include <boost/property_tree/ptree.hpp>

std::string t_encode(boost::property_tree::ptree tree);

boost::property_tree::ptree t_decode(std::string json);

class database
{
public:
  database(std::string host, std::string user, std::string password);
  ~database();
  void uc_add(std::string username, std::string password, user_card uc);
  void uc_save(std::string username, user_card uc);
  user_card auth(std::string username, std::string password, int *status);
  void add_map(map_t map, engine::Scene *s);
  void save_map(map_t map, engine::Scene *s);
  engine::Scene *load_map(map_t map, int *status);
private:
  pqxx::connection *conn;
};

database *db_init();

#endif
