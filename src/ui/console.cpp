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
  nk_layout_row_static(ctx, 30, 80, 1);
  nk_label(ctx, "Text:", NK_TEXT_LEFT);
  nk_edit_string(ctx, NK_EDIT_SIMPLE, buf, &len, CONSOLE_BUF_SIZE - 1, nk_filter_default);
  if(nk_button_label(ctx, "Enter"))
  {
    std::string line = std::string(buf, len);
    BOOST_LOG_TRIVIAL(trace) << line;
  }
  nk_end(ctx);
}
