#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include "ui/ui.h"

#define CONSOLE_BUF_SIZE 2048

class UI_console : public UI_element
{
public:
  UI_console();
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  char buf[CONSOLE_BUF_SIZE];
  int len;
};

#endif // UI_CONSOLE_H
