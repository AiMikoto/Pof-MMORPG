#ifndef CLIENT_SHUTDOWN_H
#define CLIENT_SHUTDOWN_H

#include <boost/thread/barrier.hpp>

void shutdown();

void shutdown(int _whatever);

extern boost::barrier main_barrier;

#endif // CLIENT_SHUTDOWN_H
