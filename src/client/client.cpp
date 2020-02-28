#include "client/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "client/ioc.h"

instance::instance(std::string host, int port)
{
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(boost::asio::ip::tcp::v4(), host, std::to_string(port));
  socket = new boost::asio::ip::tcp::socket(ioc);
  boost::asio::connect(*socket, endpoint);
  BOOST_LOG_TRIVIAL(info) << "connected to " << socket -> remote_endpoint().address().to_string();
  ept.add_err(boost::bind(&instance::terminate_force, this, _1));
  ept.add(OP_PING, boost::bind(&instance::handle_ping, this, _1));
  ept.add(OP_PONG, boost::bind(&instance::handle_pong, this, _1));
  ept.add(OP_TERMINATE, boost::bind(&instance::terminate, this, _1));
  boost::thread t_routine(boost::bind(&instance::routine, this));
}

instance::~instance()
{
  socket -> close();
  delete socket;
}

void instance::close()
{
  BOOST_LOG_TRIVIAL(info) << "ending connection";
  call c;
  c.tree().add(OPCODE, OP_TERMINATE);
  write_call(socket, c);
}

void instance::routine()
{
  try
  {
    forever
    {
      BOOST_LOG_TRIVIAL(trace) << "attempting to read call";
      call c = read_call(socket);
      BOOST_LOG_TRIVIAL(trace) << "received call";
      std::string opc = c.tree().get<std::string>(OPCODE);
      BOOST_LOG_TRIVIAL(trace) << "received call " << opc;
      ept.look_up(opc, c);
    }
  }
  catch(std::exception &e)
  {
    BOOST_LOG_TRIVIAL(debug) << "exception thrown - " << e.what();
    call answer;
    answer.tree().add(OPCODE, OP_TERMINATE);
    try
    {
      write_call(socket, answer);
    }
    catch(std::exception &_e)
    {
      BOOST_LOG_TRIVIAL(trace) << "exception thrown - " << _e.what();
    }
  }
}

void instance::periodic()
{
  BOOST_LOG_TRIVIAL(info) << "sending ping";
  call ping;
  ping.tree().add(OPCODE, OP_PING);
  write_call(socket, ping);
}

void instance::handle_ping(call c)
{
  BOOST_LOG_TRIVIAL(info) << "received ping";
  c.tree().add(OPCODE, OP_PONG);
  write_call(socket, c);
}

void instance::handle_pong(call c)
{
  BOOST_LOG_TRIVIAL(info) << "received pong";
}

//termination by remote host
void instance::terminate(call c)
{
  throw std::logic_error("connection closed by remote host");
}

// called when protocol is violated
void instance::terminate_force(call c)
{
  throw std::logic_error("protocol violation");
}
