#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "lib/protocol.h"

class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock);
  ~instance();
  void periodic();
private:
};

instance *instance_builder(std::string host, int port);

#endif // CLIENT_CLIENT_H
