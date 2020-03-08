#ifndef LIB_UUID_H
#define LIB_UUID_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

extern boost::uuids::random_generator generator;

std::string get_uuid();

#endif
