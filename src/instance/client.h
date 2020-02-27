#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include <boost/asio.hpp>
#include "lib/call.h"

class client
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  void routine();
  void terminate_force(call c);
  boost::asio::ip::tcp::socket *socket;
  endpoint_table ept;
};

#endif
