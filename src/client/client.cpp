#include "client/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include "include/common_macro.h"
#include <exception>
#include "client/ioc.h"

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
  try
  {
    write_call(socket, c);
  }
  catch(std::exception &e)
  {
    
    this -> close();
  }
  lock.lock();
  lock.lock();
  lock.unlock();
}

void instance::authenticate_cb(std::mutex *lock, bool *status, call c)
{
  ept.remove(OP_AUTH);
  *status = c.tree().get<bool>("success");
  lock -> unlock();
}

void instance::periodic()
{
  BOOST_LOG_TRIVIAL(info) << "sending ping";
  call ping;
  ping.tree().put(OPCODE, OP_PING);
  write_call(socket, ping);
}
