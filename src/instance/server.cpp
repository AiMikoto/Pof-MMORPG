#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "instance/server.h"
#include "include/common_macro.h"
#include "instance/ioc.h"
#include "instance/game.h"

server::server(int port):endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint)
{
  BOOST_LOG_TRIVIAL(info) << "Created listener on port " << port;
  boost::thread t_routine(boost::bind(&server::routine, this));
  boost::thread t_cleanup(boost::bind(&server::cleanup, this));
}

void server::routine()
{
  forever
  {
    boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(ioc);
    acceptor.accept(*socket);
    client *c = new client(socket);
    clients.push_back(c);
  }
}

void server::cleanup()
{
  forever
  {
    boost::this_thread::sleep( boost::posix_time::seconds(2));
    BOOST_LOG_TRIVIAL(trace) << "cleaning up";
    for(auto it = clients.begin(); it != clients.end(); it++)
    {
      client *c = *it;
      BOOST_LOG_TRIVIAL(trace) << "checking client";
      if(c -> get_ping() == -1)
      {
        BOOST_LOG_TRIVIAL(info) << "cleaning client";
        if(ucl.contains(c -> username))
        {
          BOOST_LOG_TRIVIAL(info) << "saving usercard";
          user_card uc = ucl.get(c -> username);
          ucl.remove(c -> username);
          db -> uc_save(c -> username, uc);
        }
        clients.erase(it);
        delete c;
        BOOST_LOG_TRIVIAL(info) << "cleaned client";
        it--;
      }
    }
  }
}
