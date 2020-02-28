#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include "lib/protocol.h"

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
  void periodic();
private:
};

#endif
