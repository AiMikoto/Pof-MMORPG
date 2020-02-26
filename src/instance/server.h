#ifndef INSTANCE_SERVER_H
#define INSTANCE_SERVER_H

#include <boost/asio.hpp>

class server
{
public:
  server(boost::asio::io_context& ioc, int port);
private:
  void routine();
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::io_context& io_context;
//  std::set<client> clients;
};

#endif // INSTANCE_SERVER_H
