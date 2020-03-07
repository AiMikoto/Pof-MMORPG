#include "lib/chat.h"

message::message(chat_target target, std::string payload)
{
  this -> target = target;
  this -> payload = payload;
}
