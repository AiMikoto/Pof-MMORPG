#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "instance/server.h"

#define forever while(1)

server::server(boost::asio::io_context& ioc, int port):io_context(ioc),endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint)
{
  boost::thread t(boost::bind(&server::routine, this));
}

void server::routine()
{
  forever
  {
    boost::asio::ip::tcp::iostream *stream = new boost::asio::ip::tcp::iostream;
    acceptor.accept(stream -> socket());
    *stream << "greetings";
  }
}
