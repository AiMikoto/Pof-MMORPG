#include "lib/call.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#define MAX_TRIAL_SEQUENT 1000

void noop(call c) {}

std::string make_string(boost::asio::streambuf& streambuf)
{
  return {boost::asio::buffers_begin(streambuf.data()),
          boost::asio::buffers_end(streambuf.data())};
}

call::call()
{
}

void call::decode(std::string enc)
{
  std::stringstream ss(enc);
  boost::property_tree::json_parser::read_json(ss, _tree);
}

std::string call::encode()
{
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, _tree);
  return ss.str();
}

boost::property_tree::ptree& call::tree()
{
  return _tree;
}

call read_call(boost::asio::ip::tcp::socket *s, crypto *cry)
{
  boost::asio::streambuf read_buffer;
  boost::asio::read(*s, read_buffer, boost::asio::transfer_exactly(sizeof(uint32_t)));
  int trials = 0;
  uint32_t length = *(uint32_t *)(make_string(read_buffer).c_str());
  while(s -> available() < length)
  {
    trials++;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    if(trials == MAX_TRIAL_SEQUENT)
    {
      throw std::logic_error("too slow connection");
    }
  }
  read_buffer.consume(sizeof(uint32_t));
  boost::asio::read(*s, read_buffer, boost::asio::transfer_exactly(length));
  call c;
  c.decode(cry -> decrypt(make_string(read_buffer)));
  return c;
}

void write_call(boost::asio::ip::tcp::socket *s, crypto *cry, call c)
{
  std::string payload = cry -> encrypt(c.encode());
  uint32_t size = payload.length();
  payload.insert(0, (char *) &size, sizeof(uint32_t));
  boost::asio::write(*s, boost::asio::buffer(payload), boost::asio::transfer_all());
}

endpoint_table::endpoint_table()
{
  err_cb = noop;
}

void endpoint_table::add(std::string endpoint, callback cb)
{
  table[endpoint] = cb;
}

void endpoint_table::add_err(callback cb)
{
  err_cb = cb;
}

void endpoint_table::remove(std::string endpoint)
{
  auto it = table.find(endpoint);
  if(it == table.end())
  {
    return;
  }
  table.erase(it);
}

void endpoint_table::look_up(std::string endpoint, call c)
{ 
  auto it = table.find(endpoint);
  if(it == table.end())
  {
    err_cb(c);
    return;
  }
  it -> second(c);
}
