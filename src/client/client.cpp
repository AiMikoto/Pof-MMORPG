#include "client/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include "include/common_macro.h"
#include <exception>
#include "client/ioc.h"
#include "common/user_card.h"

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
  // TODO: transfer to instance
  return status;
}

void instance::authenticate_cb(std::mutex *lock, bool *status, call c)
{
  ept.remove(OP_AUTH);
  *status = c.tree().get<bool>("success");
  BOOST_LOG_TRIVIAL(trace) << "authentification: " << *status;
  if(*status)
  {
    ept.add(OP_UC_TRANS_ALL, boost::bind(&instance::uc_transfer, this, _1));
    ept.add(OP_UC_TRANS, boost::bind(&instance::uc_transfer, this, _1));
  }
  lock -> unlock();
}

void instance::uc_transfer(call c)
{
  user_card uc;
  uc.tree() = c.tree().get_child("data");  
  BOOST_LOG_TRIVIAL(trace) << "received user card: " << uc.tree().get<std::string>("user.name");
  // TODO: store user card
}
