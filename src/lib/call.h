#ifndef LIB_CALL_H
#define LIB_CALL_H

#include <boost/property_tree/ptree.hpp>
#include <boost/asio.hpp>
#include <string>
#include <iostream>

class call
{
public:
  call();
  void decode(std::string enc);
  std::string encode();
  boost::property_tree::ptree& tree();
private:
  boost::property_tree::ptree _tree;
};

call read_call(boost::asio::ip::tcp::socket *s);

void write_call(boost::asio::ip::tcp::socket *s, call c);

#endif // LIB_CALL_H
