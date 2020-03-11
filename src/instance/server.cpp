#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "instance/server.h"
#include "include/common_macro.h"
#include "instance/ioc.h"
#include "instance/game.h"

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
  int ticks = 0;
  int refresh_rate = 2;
  forever_until(that -> shutdown)
  {
    boost::this_thread::sleep( boost::posix_time::seconds(refresh_rate));
    BOOST_LOG_TRIVIAL(trace) << "cleaning up";
    for(auto it = that -> clients.begin(); it != that -> clients.end(); it++)
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
          // TODO: unsubscribe client from irc
          db -> uc_save(c -> username, uc);
        }
        that -> clients.erase(it);
        delete c;
        BOOST_LOG_TRIVIAL(info) << "cleaned client";
        it--;
      }
    }
    if(is_loaded())
    {
      if(ticks == 300 / refresh_rate)
      {
        BOOST_LOG_TRIVIAL(trace) << "deactivating instance";
        call c;
        c.tree().put(OPCODE, OP_INSTANCE_MANAGEMENT_DEACTIVATE);
        master -> safe_write(c);
      }
      if(ticks == 360 / refresh_rate)
      {
        unload();
        ticks = 0;
      }
      ticks++;
      BOOST_LOG_TRIVIAL(trace) << "tick " << ticks;
      if(ucl.size())
      {
        if(ticks > 300 / refresh_rate)
        {
          call c;
          c.tree().put(OPCODE, OP_INSTANCE_MANAGEMENT_REACTIVATE);
          master -> safe_write(c);
        }
        ticks = 0;
      }
    }
  }
  BOOST_LOG_TRIVIAL(trace) << "cleaning clients";
  for(auto it = that -> clients.begin(); it != that -> clients.end(); it++)
  {
    client *c = *it;
    if(ucl.contains(c -> username))
    {
      BOOST_LOG_TRIVIAL(info) << "saving usercard";
      user_card uc = ucl.get(c -> username);
      ucl.remove(c -> username);
      // TODO: unsubscribe client from irc
      db -> uc_save(c -> username, uc);
    }
    BOOST_LOG_TRIVIAL(trace) << "deleting client";
    delete c;
  }
  BOOST_LOG_TRIVIAL(trace) << "cleanup terminated";
}
