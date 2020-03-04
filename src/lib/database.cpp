#include "lib/database.h"
#include "lib/crypto.h"
#include <cstdio>

void sanitize(std::string& sequence)
{
}

database::database(std::string host, std::string user, std::string password)
{
  std::string query = "host=" + host + " port=5432 user=" + user + " dbname=pof password=" + password + "";
  conn = PQconnectdb(query.c_str());
  if(PQstatus(conn) == CONNECTION_BAD)
  {
    fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
  }
}

database::~database()
{
  PQfinish(conn);
}

// return usercard
user_card database::auth(std::string username, std::string password)
{
  user_card uc;
  sanitize(username);
  std::string query = "SELECT * FROM userinfo WHERE username=\'" + username + "\' AND passwordhash=\'" + sha256(password) + "\'";
  PGresult *res = PQexec(conn, query.c_str());
  return uc;
}
