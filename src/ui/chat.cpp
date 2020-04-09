#include "ui/chat.h"
#include "lib/log.h"

UI_chat::UI_chat(chat_log *cl, UI_linear_callback callback)
{
  this -> cl = cl;
  this -> cb = callback;
  BOOST_LOG_TRIVIAL(trace) << "Defining console";
  history[0] = ""; // root
  history[1] = ""; // currently edited
  history_index = 1;
  load_from_history();
}

void UI_chat::init(ctx_t *ctx)
{
  draw(ctx);
}

void UI_chat::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_chat::render_message(ctx_t *ctx, message m)
{
}

void UI_chat::draw(ctx_t *ctx)
{
  nk_begin(ctx, "Chat", nk_rect(0, 50, 1000, 80), NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE);
  nk_layout_row_static(ctx, 30, 970, 10);
  std::deque<message> messages = cl -> get();
  for(auto m : messages)
  {
    render_message(ctx, m);
  }
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

void UI_chat::load_from_history()
{ 
  memset(buf, 0, CONSOLE_BUF_SIZE);
  len = history[history_index].size();
  memcpy(buf, history[history_index].c_str(), len);
}
