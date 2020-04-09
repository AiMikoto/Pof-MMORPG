#include "ui/console.h"
#include "lib/log.h"

UI_console::UI_console()
{
  BOOST_LOG_TRIVIAL(trace) << "Defining console";
  memset(buf, 0, CONSOLE_BUF_SIZE);
  len = 0;
}

void UI_console::init(ctx_t *ctx)
{
  draw(ctx);
}

void UI_console::visit(ctx_t *ctx)
{
  if(nk_window_is_closed(ctx, "Console"))
  {
    BOOST_LOG_TRIVIAL(trace) << "Killing console";
    suicide = true;
  }
  else
  {
    draw(ctx);
  }
}

void UI_console::draw(ctx_t *ctx)
{
  nk_begin(ctx, "Console", nk_rect(50, 50, 220, 220), NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE);
  nk_layout_row_static(ctx, 30, 170, 0);
  nk_label(ctx, "Text:", NK_TEXT_LEFT);
  nk_flags event = nk_edit_string(ctx, NK_EDIT_FIELD|NK_EDIT_SIG_ENTER, buf, &len, CONSOLE_BUF_SIZE - 1, nk_filter_default);
  if(event & NK_EDIT_COMMITED)
  {
    std::string line = std::string(buf, len);
    memset(buf, 0, CONSOLE_BUF_SIZE);
    len = 0;
    BOOST_LOG_TRIVIAL(trace) << line;
  }
  if(event & NK_EDIT_KEY_UP_PRESSED)
  {
    BOOST_LOG_TRIVIAL(trace) << "key_up happened";
  }
  nk_end(ctx);
}
