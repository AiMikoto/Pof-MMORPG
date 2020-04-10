#ifndef UI_CHAT_H
#define UI_CHAT_H

#include "ui/ui.h"
#include "lib/chat.h"
#include <map>
#include <string>
#include <boost/function.hpp>

#define CONSOLE_BUF_SIZE 2048

typedef boost::function<void(std::string)> UI_linear_callback;

class UI_chat : public UI_element
{
public:
  UI_chat(chat_log *cl, UI_linear_callback);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void render_message(ctx_t *ctx, message m);
  void draw(ctx_t *ctx);
  void load_from_history();
  char buf[CONSOLE_BUF_SIZE];
  int len;
  int history_index;
  std::map<int, std::string> history;
  UI_linear_callback cb;
  chat_log *cl;
};

#endif // UI_CHAT_H
