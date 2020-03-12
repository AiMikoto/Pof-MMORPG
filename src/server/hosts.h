#ifndef SERVER_HOST_H
#define SERVER_HOST_H

#include <boost/property_tree/ptree.hpp>
#include "include/regions.h"

extern boost::property_tree::ptree hosts;

void add(std::string type, region_t reg, std::string host, int port, std::string token);

void init_hosts();

void take_down(std::string target);

void clear_hosts();

#endif // SERVER_HOST_H
