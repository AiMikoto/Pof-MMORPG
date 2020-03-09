#ifndef LIB_DATABASE_H
#define LIB_DATABASE_H

#include <string>
#include "common/user_card.h"
#include <pqxx/pqxx>

class database
{
public:
  database(std::string host, std::string user, std::string password);
  ~database();
  void uc_add(std::string username, std::string password, user_card uc);
  void uc_save(std::string username, user_card uc);
  user_card auth(std::string username, std::string password, int *status);
private:
  pqxx::connection *conn;
};

database *db_init();

#endif
