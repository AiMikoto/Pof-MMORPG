#ifndef CHAT_SERVER_SHUTDOWN_H
#define CHAT_SERVER_SHUTDOWN_H

#include <boost/thread/barrier.hpp>

void shutdown();

void shutdown(int _whatever);

extern boost::barrier main_barrier;

#endif // CHAT_SERVER_SHUTDOWN_H
