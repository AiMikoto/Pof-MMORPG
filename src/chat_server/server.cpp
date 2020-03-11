#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "chat_server/server.h"
#include "include/common_macro.h"
#include "chat_server/ioc.h"

server::server(int port):endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint), bar(3)
{
  shutdown = false;
  BOOST_LOG_TRIVIAL(info) << "Created listener on port " << port;
  boost::thread t_routine(boost::bind(&server::routine, this));
  boost::thread t_cleanup(boost::bind(&server::cleanup, this));
}

server::~server()
{
  shutdown = true;
  bar.wait();
}

void server::routine()
{
  forever_until(shutdown)
  {
    boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(ioc);
    acceptor.accept(*socket);
    client *c = new client(socket);
    clients.push_back(c);
  }
  bar.wait();
}

void server::cleanup()
{
  forever_until(shutdown)
  {
    boost::this_thread::sleep( boost::posix_time::seconds(20));
    BOOST_LOG_TRIVIAL(trace) << "cleaning up";
    for(auto it = clients.begin(); it != clients.end(); it++)
    {
      client *c = *it;
      BOOST_LOG_TRIVIAL(trace) << "checking client";
      if(c -> get_ping() == -1)
      {
        BOOST_LOG_TRIVIAL(info) << "cleaning client";
        clients.erase(it);
        delete c;
        BOOST_LOG_TRIVIAL(info) << "cleaned client";
        it--;
      }
    }
  }
  for(auto it = clients.begin(); it != clients.end(); it++)
  {
    client *c = *it;
    BOOST_LOG_TRIVIAL(trace) << "deleting client";
    delete c;
  }
  bar.wait();
}
