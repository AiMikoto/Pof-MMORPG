#ifndef LIB_LOG_INIT
#define LIB_LOG_INIT

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

void log_init(std::string name);

void log_test();

#endif // LIB_LOG_INIT
