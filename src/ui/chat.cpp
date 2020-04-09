#include "ui/chat.h"
#include "lib/log.h"
#include <boost/range/adaptor/reversed.hpp>

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
  nk_labelf(ctx, NK_TEXT_LEFT, "[%s]%s", chat_target_as_string.at(m.target).c_str(), m.payload.c_str());
}

void UI_chat::draw(ctx_t *ctx)
{
  nk_begin(ctx, "Chat", nk_rect(50, 50, 800, 600), NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE);
  struct nk_rect bounds = nk_window_get_content_region (ctx);
  float wdth = bounds.w;
  float hght = bounds.h;
  std::deque<message> messages = cl -> get();
  nk_layout_row_static(ctx, hght - 40, wdth, 1);
  nk_group_begin(ctx, "Chat_log", 0);
  nk_layout_row_dynamic(ctx, 18, 1);
  for(auto m : boost::adaptors::reverse(messages))
  {
    render_message(ctx, m);
  }
  nk_group_end(ctx);
  nk_layout_row_dynamic(ctx, 23, 1);
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
