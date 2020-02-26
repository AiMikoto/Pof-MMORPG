#include "lib/call.h"
#include <boost/property_tree/json_parser.hpp>

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

call read_call(boost::asio::ip::tcp::socket *s)
{
  boost::asio::streambuf read_buffer;
  boost::asio::read(*s, read_buffer, boost::asio::transfer_exactly(sizeof(uint32_t)));
  uint32_t length = *(uint32_t *)(make_string(read_buffer).c_str());
  read_buffer.consume(sizeof(uint32_t));
  boost::asio::read(*s, read_buffer, boost::asio::transfer_exactly(length));
  call c;
  c.decode(make_string(read_buffer));
  return c;
}

void write_call(boost::asio::ip::tcp::socket *s, call c)
{
  std::string payload = c.encode();
  uint32_t size = payload.length();
  payload.insert(0, (char *) &size, sizeof(uint32_t));
  boost::asio::write(*s, boost::asio::buffer(payload), boost::asio::transfer_all());
}
