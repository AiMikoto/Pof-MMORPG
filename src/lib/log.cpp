#include "lib/log.h"

#include <iostream>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

void log_init(std::string name)
{
  std::string file = name + ".log";
  boost::log::add_common_attributes();
  std::string format = "[%TimeStamp%](%ThreadID%)> %Message%";
  boost::log::add_file_log(file, boost::log::keywords::format = format);
  boost::log::add_console_log(std::cout, boost::log::keywords::format = format);
  boost::log::core::get()->set_filter
  (
    boost::log::trivial::severity >= boost::log::trivial::trace
  );
  BOOST_LOG_TRIVIAL(info) << "enabled logging on file " << file;
}

void log_test()
{
  boost::log::core::get() -> set_logging_enabled(false);
}
