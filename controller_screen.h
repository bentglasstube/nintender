#pragma once

#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "screen.h"

#include <string>
#include <unordered_set>

class ControllerScreen : public Screen {
  public:

    ControllerScreen();

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    enum class Console { NES, SNES };

    std::unordered_map<Console, Backdrop> backdrops_;
    Sprite press_;
    Console console_;
    std::unordered_set<Input::Button> active_buttons_;

    struct Point {
      int x, y;
    };

    struct ConsoleConfig {
      std::string image;
      std::unordered_map<Input::Button, Point> placements;
    };

    static const std::unordered_map<Console, ConsoleConfig> kConfig;
};
