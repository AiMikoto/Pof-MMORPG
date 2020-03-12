#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "server/server.h"
#include "include/common_macro.h"
#include "server/ioc.h"

server::server(int port):endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint)
{
  shutdown = false;
  BOOST_LOG_TRIVIAL(info) << "Created listener on port " << port;
  t_routine = new boost::thread(boost::bind(routine, this));
  t_cleanup = new boost::thread(boost::bind(cleanup, this));
}

server::~server()
{
  shutdown = true;
  BOOST_LOG_TRIVIAL(trace) << "cancelling acceptor";
  acceptor.cancel();
  BOOST_LOG_TRIVIAL(trace) << "closing acceptor";
  acceptor.close();
  BOOST_LOG_TRIVIAL(trace) << "stopping ioc";
  ioc.stop();
  BOOST_LOG_TRIVIAL(trace) << "waiting on routine thread";
  t_routine -> join();
  delete t_routine;
  BOOST_LOG_TRIVIAL(trace) << "waiting on cleanup thread";
  t_cleanup -> join();
  delete t_cleanup;
  BOOST_LOG_TRIVIAL(trace) << "server terminated successfully";
}

void accept(const boost::system::error_code& error)
{
  BOOST_LOG_TRIVIAL(trace) << "accepting client";
  ioc.stop();
}

void routine(server *that)
{
  forever_until(that -> shutdown)
  {
    BOOST_LOG_TRIVIAL(trace) << "creating new socket";
    boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(ioc);
    BOOST_LOG_TRIVIAL(trace) << "adding handler";
    that -> acceptor.async_accept(*socket, boost::bind(accept, boost::asio::placeholders::error));
    BOOST_LOG_TRIVIAL(trace) << "running ioc";
    ioc.run();
    BOOST_LOG_TRIVIAL(trace) << "restarting ioc";
    ioc.restart();
    if(that -> shutdown)
    {
      break;
    }
    BOOST_LOG_TRIVIAL(trace) << "creating client";
    client *c = new client(socket);
    that -> clients.push_back(c);
  }
  BOOST_LOG_TRIVIAL(trace) << "server routine terminated";
}

void cleanup(server *that)
{
  forever_until(that -> shutdown)
  {
    boost::this_thread::sleep( boost::posix_time::seconds(10));
    BOOST_LOG_TRIVIAL(trace) << "cleaning up";
    for(auto it = that -> clients.begin(); it != that -> clients.end(); it++)
    {
      client *c = *it;
      BOOST_LOG_TRIVIAL(trace) << "checking client";
      if(c -> get_ping() == -1)
      {
        BOOST_LOG_TRIVIAL(info) << "cleaning client";
        that -> clients.erase(it);
        delete c;
        BOOST_LOG_TRIVIAL(info) << "cleaned client";
        it--;
      }
    }
  }
  BOOST_LOG_TRIVIAL(trace) << "cleaning clients";
  for(auto it = that -> clients.begin(); it != that -> clients.end(); it++)
  {
    client *c = *it;
    BOOST_LOG_TRIVIAL(trace) << "deleting client";
    delete c;
  }
  BOOST_LOG_TRIVIAL(trace) << "cleanup terminated";
}
