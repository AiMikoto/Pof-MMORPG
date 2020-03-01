#include "client/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include "include/common_macro.h"
#include <exception>
#include "client/ioc.h"
#include "common/user_card.h"
#include "client/game.h"

instance *instance_builder(std::string host, int port)
{
  boost::asio::ip::tcp::socket *sock = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), host, std::to_string(port));
  boost::asio::connect(*sock, endpoint);
  return new instance(sock);
}

instance::instance(boost::asio::ip::tcp::socket *sock):protocol(sock)
{
  BOOST_LOG_TRIVIAL(info) << "connected to " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_UC_TRANS_ALL, boost::bind(&instance::uc_transfer, this, _1));
  ept.add(OP_UC_TRANS, boost::bind(&instance::uc_transfer, this, _1));
  ept.add(OP_MOVE, boost::bind(&instance::move_cb, this, _1));
}

instance::~instance()
{
}

bool instance::authenticate(std::string username, std::string password)
{
  std::mutex lock;
  bool status;
  call c;
  ept.add(OP_AUTH, boost::bind(&instance::authenticate_cb, this, &lock, &status, _1));
  c.tree().put(OPCODE, OP_AUTH);
  c.tree().put("login.username", username);
  c.tree().put("login.password", password);
  safe_write(c);
  lock.lock();
  lock.lock();
  lock.unlock();
  return status;
}

bool instance::authenticate_token(std::string username, std::string tok)
{
  std::mutex lock;
  bool status;
  call c;
  ept.add(OP_AUTH_TOKEN, boost::bind(&instance::authenticate_cb, this, &lock, &status, _1));
  c.tree().put(OPCODE, OP_AUTH_TOKEN);
  c.tree().put("login.username", username);
  c.tree().put("login.token", tok);
  safe_write(c);
  lock.lock();
  lock.lock();
  lock.unlock();
  return status;
}

void instance::authenticate_cb(std::mutex *lock, bool *status, call c)
{
  *status = c.tree().get<bool>("status");
  BOOST_LOG_TRIVIAL(trace) << "authentication: " << *status;
  lock -> unlock();
}

bool instance::change_map(map_t map, region_t region)
{
  // TODO: make this obsolete.
  std::mutex lock;
  bool status;
  call c;
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::change_map_cb, this, &lock, &status, _1));
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP);
  c.tree().put("target.map", map);
  c.tree().put("target.region", region);
  safe_write(c);
  lock.lock();
  lock.lock();
  lock.unlock();
  return status;
}

bool instance::change_map(std::string instance_uuid)
{
  std::mutex lock;
  bool status;
  call c;
  ept.add(OP_REQUEST_CHANGE_MAP, boost::bind(&instance::change_map_cb, this, &lock, &status, _1));
  c.tree().put(OPCODE, OP_REQUEST_CHANGE_MAP);
  c.tree().put("target.uuid", instance_uuid);
  safe_write(c);
  lock.lock();
  lock.lock();
  lock.unlock();
  return status;
}

void instance::change_map_cb(std::mutex *lock, bool *status, call c)
{
  ept.remove(OP_REQUEST_CHANGE_MAP);
  *status = c.tree().get<bool>("status");
  BOOST_LOG_TRIVIAL(trace) << "map_change_request: " << *status;
  lock -> unlock();
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
