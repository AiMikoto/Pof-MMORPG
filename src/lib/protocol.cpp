#include "lib/protocol.h"
#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>

protocol::protocol(boost::asio::ip::tcp::socket *sock)
{
  socket = sock;
  ept.add_err(boost::bind(&protocol::terminate_force, this, _1));
  ept.add(OP_PING, boost::bind(&protocol::handle_ping, this, _1));
  ept.add(OP_PONG, boost::bind(&protocol::handle_pong, this, _1));
  ept.add(OP_TERMINATE, boost::bind(&protocol::terminate, this, _1));
  t_routine = new boost::thread(boost::bind(&protocol::routine, this));
}

protocol::~protocol()
{
  socket -> close();
  delete socket;
  if(t_routine)
  { // might have been stopped by protocol::close
    delete t_routine;
  }
}

void protocol::close()
{
  BOOST_LOG_TRIVIAL(info) << "ending connection";
  call c;
  c.tree().add(OPCODE, OP_TERMINATE);
  write_call(socket, c);
  delete t_routine;
  t_routine = NULL;
}

void protocol::routine()
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

void protocol::handle_ping(call c)
{
  BOOST_LOG_TRIVIAL(info) << "received ping";
  c.tree().add(OPCODE, OP_PONG);
  write_call(socket, c);
}

void protocol::handle_pong(call c)
{
  BOOST_LOG_TRIVIAL(info) << "received pong";
}

//termination by remote host
void protocol::terminate(call c)
{
  throw std::logic_error("connection closed by remote host");
}

// called when protocol is violated
void protocol::terminate_force(call c)
{
  throw std::logic_error("protocol violation");
}
