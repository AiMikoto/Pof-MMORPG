#include "lib/database.h"
#include "lib/crypto.h"
#include <pqxx/pqxx>
#include <boost/property_tree/json_parser.hpp>

std::string t_encode(boost::property_tree::ptree tree)
{
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, tree);
  return ss.str();
}

boost::property_tree::ptree t_decode(std::string json)
{
  boost::property_tree::ptree tree;
  std::stringstream ss(json);
  boost::property_tree::json_parser::read_json(ss, tree);
  return tree;
}

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
  conn = new pqxx::connection(query);
}

database::~database()
{
  delete conn;
}

void database::uc_add(std::string username, std::string password, user_card uc)
{
  sanitize(username);
  std::string query = "INSERT INTO userinfo(usercard, username, passwordhash) VALUES(\'" + uc.save() + "\', \'" + username + "\', \'" + sha256(password) + "\')";
  pqxx::work W{*conn};
  W.exec(query);
}

void database::uc_save(std::string username, user_card uc)
{
  sanitize(username);
  std::string query = "UPDATE userinfo SET usercard = \'" + uc.save() + "\' WHERE username = \'" + username + "\'";
  pqxx::work W{*conn};
  W.exec0(query);
  W.commit();
}

// return usercard
user_card database::auth(std::string username, std::string password, int *status)
{
  user_card uc;
  sanitize(username);
  std::string query = "SELECT usercard FROM userinfo WHERE username=\'" + username + "\' AND passwordhash=\'" + sha256(password) + "\'";
  pqxx::work W{*conn};
  pqxx::result R{W.exec(query)};
  *status = R.size();
  if(*status)
  {
    std::string data = std::string(R[0][0].c_str());
    uc.load(data);
  }
  return uc;
}

void database::map_add(map_t map, engine::Scene *s)
{
  std::string query = "INSERT INTO mapinfo(data, name) VALUES(\'" + t_encode(s -> serialize()) + "\', \'" + map + "\')";
  pqxx::work W{*conn};
  W.exec(query);
}

void database::map_save(map_t map, engine::Scene *s)
{
  std::string query = "UPDATE mapinfo SET data = \'" + t_encode(s -> serialize()) + "\' WHERE name = \'" + map + "\'";
  pqxx::work W{*conn};
  W.exec0(query);
  W.commit();
}

engine::Scene *database::load_map(map_t map, int *status)
{
  engine::Scene *s;
  std::string query = "SELECT data FROM mapinfo WHERE name=\'" + map + "\'";
  pqxx::work W{*conn};
  pqxx::result R{W.exec(query)};
  *status = R.size();
  if(*status)
  {
    std::string data = std::string(R[0][0].c_str());
    s = new engine::Scene(t_decode(data));
  }
  return s;
}
