#ifndef LIB_CALL_H
#define LIB_CALL_H

#include <boost/property_tree/ptree.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>
#include <iostream>
#include <map>
#include "lib/crypto.h"

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

call read_call(boost::asio::ip::tcp::socket *s, crypto *cry);

void write_call(boost::asio::ip::tcp::socket *s, call c, crypto *cry);

typedef boost::function<void(call)> callback;

class endpoint_table
{
public:
  endpoint_table();
  void add(std::string endpoint, callback cb);
  void add_err(callback cb);
  void remove(std::string endpoint);
  void look_up(std::string endpoint, call c);
private:
  std::map<std::string, callback> table;
  callback err_cb;
};


#endif // LIB_CALL_H
