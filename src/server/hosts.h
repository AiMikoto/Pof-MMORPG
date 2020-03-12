#ifndef SERVER_HOST_H
#define SERVER_HOST_H

#include <boost/property_tree/ptree.hpp>

extern boost::property_tree::ptree hosts;

void init_hosts();

void clear_hosts();

#endif // SERVER_HOST_H
