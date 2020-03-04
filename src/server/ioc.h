#ifndef SERVER_IOC_H
#define SERVER_IOC_H

#include <boost/asio.hpp>
#include "lib/database.h"

extern boost::asio::io_context ioc;
extern database *db;

#endif // SERVER_IOC_H
