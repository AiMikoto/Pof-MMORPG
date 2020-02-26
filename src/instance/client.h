#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include <boost/asio.hpp>

class client
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  boost::asio::ip::tcp::socket *socket;
};

#endif
