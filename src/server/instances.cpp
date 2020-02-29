#include "server/instances.h"
#include "server/ioc.h"

std::map<int, std::map<std::string, public_instance*>> pins;

instance *instance_builder(instance_info ini)
{
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), ini.hostname, std::to_string(ini.port));
  boost::asio::connect(*sock, endpoint);
  return new instance(sock);
}

instance::instance(boost::asio::ip::tcp::socket *sock):protocol(sock, -1)
{
}

instance_info::instance_info(std::string auth_tok, std::string hostname, int port)
{
  this -> auth_tok = auth_tok;
  this -> hostname = hostname;
  this -> port = port;
}

public_instance::public_instance(std::string auth_tok, std::string hostname, int port, int channel, std::string map):instance_info(auth_tok, hostname, port)
{
  this -> in = instance_builder(*this);
  // TODO: connect to instance and load map
}

void populate_pins()
{
  // TODO: spawn servers
  public_instance *pi = new public_instance("fish", "localhost", 7000, REG_EU, "flatlands");
  pins[REG_EU]["flatlands"] = pi;
}
