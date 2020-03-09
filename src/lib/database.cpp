#include "lib/database.h"
#include "lib/crypto.h"

void sanitize(std::string& sequence)
{
  // TODO: add sanitisation
}

database *db_init()
{
  return new database("iarba.ga", "postgres", "pof");
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

void database::uc_add(std::string username, std::string password, user_card uc)
{
  sanitize(username);
  std::string query = "INSERT INTO userinfo(usercard, username, passwordhash) VALUES(\'" + uc.save() + "\', \'" + username + "\', \'" + sha256(password) + "\')";
  PGresult *res = PQexec(conn, query.c_str());
  PQclear(res);
}

void database::uc_save(std::string username, user_card uc)
{ // TODO: optimise this
  sanitize(username);
  std::string query = "UPDATE userinfo SET usercard = \'" + uc.save() + "\' WHERE username = \'" + username + "\'";
  PGresult *res = PQexec(conn, query.c_str());
  PQclear(res);
}

// return usercard
user_card database::auth(std::string username, std::string password, int *status)
{
  user_card uc;
  sanitize(username);
  std::string query = "SELECT usercard FROM userinfo WHERE username=\'" + username + "\' AND passwordhash=\'" + sha256(password) + "\'";
  PGresult *res = PQexec(conn, query.c_str());
  *status = PQntuples(res);
  if(*status)
  {
    uc.load(std::string(PQgetvalue(res, 0, 0)));
  }
  PQclear(res);
  return uc;
}
