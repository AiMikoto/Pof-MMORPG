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
  user_card auth(std::string username, std::string password);
private:
  PGconn *conn;
};

#endif
