#include "ui/console.h"
#include "lib/log.h"

UI_console::UI_console(UI_console_callback callback)
{
  this -> cb = callback;
  BOOST_LOG_TRIVIAL(trace) << "Defining console";
  history[0] = ""; // root
  history[1] = ""; // currently edited
  history_index = 1;
  load_from_history();
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
    history_index = history.size(); // return to present
    if(history[history_index - 2] == line)
    {
      history_index--;
    }
    history[history_index - 1] = line; // last edited
    history[history_index] = ""; // currently edited
    load_from_history();
    cb(line);
  }
  if(event & NK_EDIT_KEY_UP_PRESSED)
  { // look into the past
    history_index = std::max(history_index - 1, 0);
    load_from_history();
  }
  if(event & NK_EDIT_KEY_DOWN_PRESSED)
  { // look into the future
    history_index = std::min(history_index + 1, (int)history.size() - 1);
    load_from_history();
  }
  nk_end(ctx);
}

void UI_console::load_from_history()
{ 
  memset(buf, 0, CONSOLE_BUF_SIZE);
  len = history[history_index].size();
  memcpy(buf, history[history_index].c_str(), len);
}
