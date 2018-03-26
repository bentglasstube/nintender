#include "game.h"

#include "controller_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "nintender";
  config.graphics.width = 268;
  config.graphics.height = 120;
  config.graphics.fullscreen = false;

  Game game(config);
  game.loop(new ControllerScreen());

  return 0;
}
