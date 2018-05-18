#pragma once

#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "screen.h"
#include "text.h"
#include "util.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

class ControllerScreen : public Screen {
  public:

    ControllerScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    enum class Console { NES, SNES, GB };
    enum class State { Waiting, Choose, Bind, Ready };

    std::unordered_map<Console, Backdrop, Util::CastHash<Console>> backdrops_;
    Sprite press_;
    Text text_;
    Console console_;
    State state_;
    std::unordered_set<Input::Button, Util::CastHash<Input::Button>> active_buttons_;
    int timer_;
    std::string name_;

    struct Point {
      int x, y;
    };

    struct ConsoleConfig {
      std::string image;
      std::unordered_map<Input::Button, Point, Util::CastHash<Input::Button>> placements;
    };

    void check_controllers();

    static const std::unordered_map<Console, ConsoleConfig, Util::CastHash<Console>> kConfig;
};
