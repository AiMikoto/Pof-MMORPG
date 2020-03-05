#ifndef LIB_DATABASE_H
#define LIB_DATABASE_H

#include <libpq-fe.h>
#include <string>
#include "common/user_card.h"

class database
{
public:
  database(std::string host, std::string user, std::string password);
  ~database();
  void uc_add(std::string username, std::string password, user_card uc);
  void uc_save(std::string username, user_card uc);
  user_card auth(std::string username, std::string password, int *status);
private:
  PGconn *conn;
};

database *db_init();

#endif
