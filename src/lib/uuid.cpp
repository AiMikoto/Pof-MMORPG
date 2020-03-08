#include "lib/uuid.h"

boost::uuids::random_generator generator;

std::string get_uuid()
{
  return boost::lexical_cast<std::string>(generator());
}
