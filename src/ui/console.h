#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include "ui/ui.h"
#include <map>
#include <string>
#include <boost/function.hpp>

#define CONSOLE_BUF_SIZE 2048

typedef boost::function<void(std::string)> UI_console_callback;

class UI_console : public UI_element
{
public:
  UI_console(UI_console_callback);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  void load_from_history();
  char buf[CONSOLE_BUF_SIZE];
  int len;
  int history_index;
  std::map<int, std::string> history;
  UI_console_callback cb;
};

#endif // UI_CONSOLE_H
