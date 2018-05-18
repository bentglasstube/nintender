#include "controller_screen.h"

ControllerScreen::ControllerScreen() : press_("press.png", 0, 0, 8, 8), text_("text.png"), console_(Console::GB) {
  for (const auto& con : kConfig) {
    backdrops_.emplace(con.first, con.second.image);
  }
}

bool ControllerScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  controller_ = input.has_gamepad();

  active_buttons_.clear();

  if (input.key_held(Input::Button::Up))     active_buttons_.insert(Input::Button::Up);
  if (input.key_held(Input::Button::Down))   active_buttons_.insert(Input::Button::Down);
  if (input.key_held(Input::Button::Left))   active_buttons_.insert(Input::Button::Left);
  if (input.key_held(Input::Button::Right))  active_buttons_.insert(Input::Button::Right);
  if (input.key_held(Input::Button::A))      active_buttons_.insert(Input::Button::A);
  if (input.key_held(Input::Button::B))      active_buttons_.insert(Input::Button::B);
  if (input.key_held(Input::Button::Start))  active_buttons_.insert(Input::Button::Start);
  if (input.key_held(Input::Button::Select)) active_buttons_.insert(Input::Button::Select);

  if (input.key_held(Input::Button::X))      active_buttons_.insert(Input::Button::X);
  if (input.key_held(Input::Button::Y))      active_buttons_.insert(Input::Button::Y);
  if (input.key_held(Input::Button::L))      active_buttons_.insert(Input::Button::L);
  if (input.key_held(Input::Button::R))      active_buttons_.insert(Input::Button::R);

  if (input.key_pressed(Input::Button::User1)) {
    // TODO better handling of console values
    switch (console_) {
      case ControllerScreen::Console::GB:
        console_ = ControllerScreen::Console::NES;
        break;

      case ControllerScreen::Console::NES:
        console_ = ControllerScreen::Console::SNES;
        break;

      case ControllerScreen::Console::SNES:
        console_ = ControllerScreen::Console::GB;
        break;
    }
  }

  return true;
}

void ControllerScreen::draw(Graphics& graphics) const {
  const auto b = backdrops_.find(console_);
  if (b == backdrops_.end()) return;
  b->second.draw(graphics);

  const auto map = kConfig.find(console_);
  if (map == kConfig.end()) return;

  for (const Input::Button b : active_buttons_) {
    const auto p = map->second.placements.find(b);
    if (p != map->second.placements.end()) {
      press_.draw(graphics, p->second.x, p->second.y);
    }
  }

  if (!controller_) {
    SDL_Rect r = { 0, 0, graphics.width(), graphics.height() };
    graphics.draw_rect(&r, 0x000000a0, true);
    text_.draw(graphics, "No supported controllers found", 134, 104, Text::Alignment::Center);
  }
}

Screen* ControllerScreen::next_screen() const {
  return nullptr;
}

const std::unordered_map<ControllerScreen::Console, ControllerScreen::ConsoleConfig, Util::CastHash<ControllerScreen::Console>> ControllerScreen::kConfig= {
  {
    ControllerScreen::Console::GB,
    {
      "gb.png",
      {
        { Input::Button::Up,     {  51,  14} },
        { Input::Button::Down,   {  51,  59} },
        { Input::Button::Left,   {  29,  36 } },
        { Input::Button::Right,  {  73,  36 } },
        { Input::Button::A,      { 223,  23 } },
        { Input::Button::B,      { 181,  43 } },
        { Input::Button::Start,  { 136,  99} },
        { Input::Button::Select, {  92,  99} },
      }
    }
  },
  {
    ControllerScreen::Console::NES,
    {
      "nes.png",
      {
        { Input::Button::Up,     {  42,  49} },
        { Input::Button::Down,   {  42,  85 } },
        { Input::Button::Left,   {  24,  67 } },
        { Input::Button::Right,  {  60,  67 } },
        { Input::Button::A,      { 219,  79 } },
        { Input::Button::B,      { 186,  79 } },
        { Input::Button::Start,  { 136,  79 } },
        { Input::Button::Select, { 102,  79 } },
      }
    }
  },
  {
    ControllerScreen::Console::SNES,
    {
      "snes.png",
      {
        { Input::Button::Up,     {  50,  42} },
        { Input::Button::Down,   {  50,  73 } },
        { Input::Button::Left,   {  34,  58} },
        { Input::Button::Right,  {  65,  58 } },
        { Input::Button::A,      { 233,  59 } },
        { Input::Button::B,      { 208,  79 } },
        { Input::Button::Start,  { 138,  64 } },
        { Input::Button::Select, { 112,  64 } },
        { Input::Button::X,      { 213,  35 } },
        { Input::Button::Y,      { 189,  56} },
        { Input::Button::L,      {  52,   3 } },
        { Input::Button::R,      { 212,   3 } },
      }
    }
  },
};
