#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "chat_server/server.h"
#include "include/common_macro.h"
#include "chat_server/ioc.h"

server::server(int port):endpoint(boost::asio::ip::tcp::v4(), port), acceptor(ioc, endpoint), bar(3), acceptor_barrier(2)
{
  shutdown = false;
  BOOST_LOG_TRIVIAL(info) << "Created listener on port " << port;
  boost::thread t_routine(boost::bind(routine, this));
  boost::thread t_cleanup(boost::bind(cleanup, this));
}

server::~server()
{
  shutdown = true;
  BOOST_LOG_TRIVIAL(trace) << "cancelling acceptor";
  acceptor.cancel();
  BOOST_LOG_TRIVIAL(trace) << "closing acceptor";
  acceptor.close();
  BOOST_LOG_TRIVIAL(trace) << "stopping io context";
  ioc.stop();
  BOOST_LOG_TRIVIAL(trace) << "lowering acceptor barrier";
  acceptor_barrier.wait();
  BOOST_LOG_TRIVIAL(trace) << "server shutdown terminated";
  bar.wait();
  BOOST_LOG_TRIVIAL(trace) << "server terminated successfully";
}

void server::accept()
{
  acceptor_barrier.wait();
}

void routine(server *that)
{
  forever_until(that -> shutdown)
  {
    boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(ioc);
    that -> acceptor.async_accept(*socket, boost::bind(&server::accept, that));
    that -> acceptor_barrier.wait();
    if(that -> shutdown)
    {
      break;
    }
    client *c = new client(socket);
    that -> clients.push_back(c);
  }
  BOOST_LOG_TRIVIAL(trace) << "server routine terminated";
  that -> bar.wait();
}

void cleanup(server *that)
{
  forever_until(that -> shutdown)
  {
    boost::this_thread::sleep( boost::posix_time::seconds(20));
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
  for(auto it = that -> clients.begin(); it != that -> clients.end(); it++)
  {
    client *c = *it;
    BOOST_LOG_TRIVIAL(trace) << "deleting client";
    delete c;
  }
  BOOST_LOG_TRIVIAL(trace) << "cleanup terminated";
  that -> bar.wait();
}
