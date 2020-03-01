#include "server/instances.h"
#include "server/ioc.h"

std::map<region_t, std::map<map_t, public_instance*>> pins;

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

void instance::handle_map_change_request(call c)
{
  region_t reg = c.tree().get<region_t>("target.region");
  map_t map = c.tree().get<map_t>("target.map");
  user_card uc;
  uc.tree() = c.tree().get_child("card");
  pins[reg][map] -> transfer_user_card(uc);
}

instance_info::instance_info(std::string auth_tok, std::string hostname, int port)
{
  this -> auth_tok = auth_tok;
  this -> hostname = hostname;
  this -> port = port;
}

void instance_info::transfer_user_card(user_card uc)
{
  call uc_transfer;
  uc_transfer.tree().put(OPCODE, OP_UC_TRANS_ALL);
  uc_transfer.tree().put_child("data", uc.tree());
  uc_transfer.tree().put("authority.token", this -> auth_tok);
  this -> in -> safe_write(uc_transfer);
}

public_instance::public_instance(std::string auth_tok, std::string hostname, int port, region_t channel, map_t map):instance_info(auth_tok, hostname, port)
{
  this -> channel = channel;
  this -> map = map;
  this -> in = instance_builder(*this);
  call init;
  init.tree().put(OPCODE, OP_CMD);
  init.tree().put("authority.token", this -> auth_tok);
  this -> in -> safe_write(init);
  // TODO: connect to instance and load map
}

void populate_pins()
{
  // TODO: spawn servers
  public_instance *pi = new public_instance("fish", "localhost", 7000, REG_EU, MAP_FLATLANDS);
  pins[REG_EU][MAP_FLATLANDS] = pi;
}
