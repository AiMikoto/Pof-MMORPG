#ifndef CLIENT_EDITOR_H
#define CLIENT_EDITOR_H

#include "client/manipulator.h"
#include <string>

class editor
{
public:
  editor(std::string hostname, int port, std::string tok);
  ~editor();
  bool try_handle(std::string line);
protected:
  bool try_ss(std::string line);
  bool try_sps(std::string line);
  bool try_move(std::string line);
  bool try_scale(std::string line);
  bool try_rotate(std::string line);
  bool try_save(std::string line);
  bool try_spawn(std::string line);
  bool try_delete(std::string line);
  bool try_add(std::string line);
private:
  manipulator *man;
};

#endif // CLIENT_EDITOR_H
