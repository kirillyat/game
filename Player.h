#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords), skin("resources/1.png") {};

  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);

private:
  Image skin; // = "resourses/tex.png");
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 0, .g = 0, .b = 254, .a = 255};
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H