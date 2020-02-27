#include "instance/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>


client::client(boost::asio::ip::tcp::socket *sock)
{
  socket = sock;
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add_err(boost::bind(&client::terminate_force, this, _1));
  boost::thread t(boost::bind(&client::routine, this));
}

client::~client()
{
  socket -> close();
  delete socket;
}

void client::routine()
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
    BOOST_LOG_TRIVIAL(debug) << "exception thrown" << e.what();
    call answer;
    answer.tree().add(OPCODE, OP_TERMINATE);
    try
    {
      write_call(socket, answer);
    }
    catch(std::exception &_e)
    {
      BOOST_LOG_TRIVIAL(trace) << "exception thrown" << _e.what();
    }
  }
  delete this;
}

// called when protocol is violated
void client::terminate_force(call c)
{
  throw std::logic_error("protocol violation");
}
