#ifndef LIB_CALL_H
#define LIB_CALL_H

#include <boost/property_tree/ptree.hpp>
#include <string>

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

#endif // LIB_CALL_H
