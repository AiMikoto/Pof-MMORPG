#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "instance/server.h"
#include "include/common_macro.h"

server::server(boost::asio::io_context& ioc, int port):io_context(ioc),endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint)
{
  BOOST_LOG_TRIVIAL(info) << "Created listener on port " << port;
  boost::thread t(boost::bind(&server::routine, this));
}

void server::routine()
{
  forever
  {
    boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(io_context);
    acceptor.accept(*socket);
    client *c = new client(socket);
    clients.push_back(c);
  }
}
