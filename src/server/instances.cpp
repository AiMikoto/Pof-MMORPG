#include "server/instances.h"
#include "server/ioc.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

rsa_crypto *g_rsa;
aes_crypto *g_aes;

void init_crypto(std::string pub, std::string priv)
{
  boost::uuids::random_generator generator;
  std::string uuid1 = boost::lexical_cast<std::string>(generator());
  std::string uuid2 = boost::lexical_cast<std::string>(generator());
  std::string uuid3 = uuid1 + uuid2;
  std::string uuid4 = boost::lexical_cast<std::string>(generator());
  g_aes = new aes_crypto(uuid3.substr(0, 32), uuid4.substr(0, 16));
  BIO *keybio1 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio1, pub.c_str());
  BIO *keybio2 = BIO_new(BIO_s_file());
  BIO_read_filename(keybio2, priv.c_str());
  g_rsa = new rsa_crypto(keybio1, keybio2);
}

std::map<region_t, std::map<map_t, public_instance*>> pins;

instance *instance_builder(instance_info ini)
{
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), ini.hostname, std::to_string(ini.port));
  boost::asio::connect(*sock, endpoint);
  return new instance(sock);
}

instance::instance(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, -1)
{
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::handle_map_change_request, this, _1));
}

void instance::handle_map_change_request(call c)
{
  region_t reg = c.tree().get<region_t>("target.region");
  map_t map = c.tree().get<map_t>("target.map");
  user_card uc;
  uc.tree() = c.tree().get_child("card");
  instance_info *instance = pins[reg][map];
  instance -> transfer_user_card(uc);
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP_CB);
  c.tree().put("status", true);
  c.tree().put("target.host", instance -> hostname);
  c.tree().put("target.port", instance -> port);
  this -> safe_write(c);
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
  init.tree().put("command", "init");
  init.tree().put("aes.key", g_aes -> key);
  init.tree().put("aes.iv", g_aes -> iv);
  this -> in -> safe_write(init); // uses RSA
  this -> in -> replace_crypto(g_aes); // chance crypto to aes
  this -> in -> start();
  // TODO: connect to instance and load map
}

void populate_pins()
{
  // TODO: spawn servers
  public_instance *pi = new public_instance("fish", "localhost", 7000, REG_EU, MAP_FLATLANDS);
  pins[REG_EU][MAP_FLATLANDS] = pi;
}
