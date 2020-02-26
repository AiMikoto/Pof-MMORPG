#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "instance/server.h"

#define forever while(1)

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
    BOOST_LOG_TRIVIAL(debug) << "received new connection from " << socket -> remote_endpoint().address().to_string();;
//    client *c = new client(stream);
  }
}
