#include "lib/protocol.h"
#include "lib/log.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "include/common_macro.h"
#include <exception>

protocol::protocol(boost::asio::ip::tcp::socket *sock)
{
  socket = sock;
  ping = 0;
  ept.add_err(boost::bind(&protocol::terminate_force, this, _1));
  ept.add(OP_PING, boost::bind(&protocol::handle_ping, this, _1));
  ept.add(OP_PONG, boost::bind(&protocol::handle_pong, this, _1));
  ept.add(OP_TERMINATE, boost::bind(&protocol::terminate, this, _1));
  t_routine = new boost::thread(boost::bind(&protocol::routine, this));
  t_pinger = new boost::thread(boost::bind(&protocol::latency_service, this));
}

protocol::~protocol()
{
  socket -> close();
  delete socket;
  t_pinger -> join();
  delete t_pinger;
  t_routine -> join();
  delete t_routine;
}

int protocol::safe_write(call c)
{
  try
  {
    // TODO: mutex
    write_call(socket, c);
  }
  catch(std::exception &e)
  {
    BOOST_LOG_TRIVIAL(trace) << "protocol::write_safe - exception thrown when writing to socket - " << e.what();
    this -> close();
    return -1;
  }
  return 0;
}

void protocol::close()
{
  BOOST_LOG_TRIVIAL(info) << "ending connection";
  call c;
  c.tree().put(OPCODE, OP_TERMINATE);
  try
  {
    // TODO: mutex
    write_call(socket, c);
  }
  catch(std::exception &e)
  {
    BOOST_LOG_TRIVIAL(trace) << "protocol::close - exception thrown when writing to socket - " << e.what();
  }
  socket -> close();
  ping = -1;
}

int protocol::get_ping()
{
  return ping;
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
    BOOST_LOG_TRIVIAL(debug) << "protocol::routine - exception thrown during routine - " << e.what();
    call answer;
    answer.tree().put(OPCODE, OP_TERMINATE);
    safe_write(answer);
    this -> close();
  }
}

void protocol::latency_service()
{
  boost::asio::io_context io;
  boost::uuids::random_generator generator;
  forever
  {
    BOOST_LOG_TRIVIAL(trace) << "sending ping";
    std::string uuid = boost::lexical_cast<std::string>(generator());
    call ping_c;
    ping_c.tree().put(OPCODE, OP_PING);
    ping_c.tree().put("ping.uuid", uuid);
    pings[uuid] = boost::chrono::high_resolution_clock::now();
    if(safe_write(ping_c))
    {
      this -> close();
      return;
    }
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.wait();
  }
}

void protocol::handle_ping(call c)
{
  BOOST_LOG_TRIVIAL(trace) << "received ping";
  c.tree().put(OPCODE, OP_PONG);
  safe_write(c);
}

void protocol::handle_pong(call c)
{
  BOOST_LOG_TRIVIAL(trace) << "received pong";
  std::string uuid = c.tree().get<std::string>("ping.uuid");
  auto i = pings.find(uuid);
  if(i != pings.end())
  {
    boost::chrono::high_resolution_clock::duration diff = boost::chrono::high_resolution_clock::now() - pings[uuid];
    ping = boost::chrono::duration_cast<boost::chrono::milliseconds>(diff).count();
    BOOST_LOG_TRIVIAL(debug) << "updated ping - " << ping;
  }
  else
  {
    throw std::logic_error("forged UUID in pong");
  }
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
