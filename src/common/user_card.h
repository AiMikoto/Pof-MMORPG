#ifndef COMMON_USER_CARD_H
#define COMMON_USER_CARD_H

#include <boost/property_tree/ptree.hpp>

class user_card
{
public:
  boost::property_tree::ptree& tree();
private:
  boost::property_tree::ptree _tree;
};

#endif // COMMON_USER_CARD_H
