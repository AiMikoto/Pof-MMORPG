#include "lib/call.h"
#include <boost/property_tree/json_parser.hpp>

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
