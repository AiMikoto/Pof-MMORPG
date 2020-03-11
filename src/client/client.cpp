#include "client/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include "include/common_macro.h"
#include <exception>
#include "client/ioc.h"
#include "common/user_card.h"
#include "client/game.h"
#include "lib/uuid.h"

rsa_crypto *g_rsa;
aes_crypto *g_aes;

void init_crypto(std::string pub)
{
  std::string uuid1 = get_uuid();
  std::string uuid2 = get_uuid();
  std::string uuid3 = uuid1 + uuid2;
  std::string uuid4 = get_uuid();
  g_aes = new aes_crypto(uuid3.substr(0, 32), uuid4.substr(0, 16));
  BIO *keybio = BIO_new(BIO_s_file());
  BIO_read_filename(keybio, pub.c_str());
  g_rsa = new rsa_crypto(keybio, NULL);
}

instance *instance_builder(std::string host, int port)
{
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), host, std::to_string(port));
  boost::asio::connect(*sock, endpoint);
  return new instance(sock);
}

instance::instance(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa)
{
  BOOST_LOG_TRIVIAL(info) << "connected to " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_UC_TRANS_ALL, boost::bind(&instance::uc_transfer, this, _1));
  ept.add(OP_UC_TRANS, boost::bind(&instance::uc_transfer, this, _1));
  ept.add(OP_MOVE, boost::bind(&instance::move_cb, this, _1));
  ept.add(OP_IRC, boost::bind(&instance::irc_cb, this, _1));
}

instance::~instance()
{
}

bool instance::authenticate(std::string username, std::string password)
{
  boost::barrier bar(2);
  bool status;
  call c;
  ept.add(OP_AUTH, boost::bind(&instance::authenticate_cb, this, &bar, &status, _1));
  c.tree().put(OPCODE, OP_AUTH);
  c.tree().put("login.username", username);
  c.tree().put("login.password", password);
  c.tree().put("aes.key", g_aes -> key);
  c.tree().put("aes.iv", g_aes -> iv);
  safe_write(c); // uses RSA
  replace_crypto(g_aes); // chance crypto to aes
  start();
  bar.wait();
  return status;
}

bool instance::authenticate_token(std::string username, std::string tok)
{
  boost::barrier bar(2);
  bool status;
  call c;
  ept.add(OP_AUTH_TOKEN, boost::bind(&instance::authenticate_cb, this, &bar, &status, _1));
  c.tree().put(OPCODE, OP_AUTH_TOKEN);
  c.tree().put("login.username", username);
  c.tree().put("login.token", tok);
  c.tree().put("aes.key", g_aes -> key);
  c.tree().put("aes.iv", g_aes -> iv);
  safe_write(c); // uses RSA
  replace_crypto(g_aes); // chance crypto to aes
  start();
  bar.wait();
  return status;
}

void instance::authenticate_cb(boost::barrier *bar, bool *status, call c)
{
  *status = c.tree().get<bool>("status");
  BOOST_LOG_TRIVIAL(trace) << "authentication: " << *status;
  bar -> wait();
}

bool instance::change_map(map_t map, region_t region)
{
  boost::barrier bar(2);
  bool status;
  call c;
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::change_map_cb, this, &bar, &status, _1));
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP);
  c.tree().put("target.map", map);
  c.tree().put("target.region", region);
  c.tree().put("target.public", true);
  safe_write(c);
  bar.wait();
  return status;
}

bool instance::change_map(std::string instance_uuid)
{
  boost::barrier bar(2);
  bool status;
  call c;
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::change_map_cb, this, &bar, &status, _1));
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP);
  c.tree().put("target.uuid", instance_uuid);
  c.tree().put("target.public", false);
  safe_write(c);
  bar.wait();
  return status;
}

void instance::send_message(message m)
{
  call c;
  c.tree().put(OPCODE, OP_IRC);
  c.tree().put_child("payload", m.encode());
  safe_write(c);
}

void instance::change_map_cb(boost::barrier *bar, bool *status, call c)
{
  ept.remove(OP_REQUEST_CHANGE_MAP);
  *status = c.tree().get<bool>("status");
  BOOST_LOG_TRIVIAL(trace) << "map_change_request: " << *status;
  bar -> wait();
}

void instance::uc_transfer(call c)
{
  user_card uc;
  uc.tree() = c.tree().get_child("data");  
  BOOST_LOG_TRIVIAL(trace) << "received user card: " << uc.tree().get<std::string>("user.name");
  ucl.add(uc);
}

void instance::move_cb(call c)
{
  std::string host = c.tree().get<std::string>("target.host");
  int port = c.tree().get<int>("target.port");
  BOOST_LOG_TRIVIAL(info) << "received instance transfer to " << host << ":" << port;
  boost::thread t(boost::bind(move, host, port));
}

void instance::irc_cb(call c)
{
  BOOST_LOG_TRIVIAL(info) << "received message";
  message m(c.tree().get_child("payload"));
  cl.add(m);
}
