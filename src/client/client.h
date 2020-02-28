#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "lib/protocol.h"

class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock);
  ~instance();
  bool authenticate(std::string username, std::string password);
private:
  void authenticate_cb(std::mutex *lock, bool *status, call c);
};

instance *instance_builder(std::string host, int port);

#endif // CLIENT_CLIENT_H
