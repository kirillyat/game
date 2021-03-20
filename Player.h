#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <fstream>
#include <iostream>
#include <string>


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
  Player (
    Point pos,
    const std::string &wall_tale_path,
    const std::string &weak_wall_tale_path,
    const std::string &floor_tale_path,
    const std::string &hole_tale_path,
    const std::string &finish_path,
    const std::string &heroSkin_path,
    int health
  );
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords), health(100) {};
                 
  void SetLevelStyle();
  void SetHeroSkin();

  
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);
  void DrawLevel(Image &screen);
  void InitLevel();

  bool Moved() const;
  bool CheckCoords();
  bool CheckPos(Point pos);
  bool CheckFin(Point pos);
  bool CheckHole(Point pos);
  void openCurrentLevelMap();
  char lookup(Point pos);

private:
  Image* wall_tale;
	Image* weak_wall_tale;
	Image* floor_tale;
	Image* hole_tale;
  Image* finish_tale;
  Image* heroSkin;
  Image* level;

  std::ifstream levelinput;
  bool wait;
  int health;
  int level_n;
  Point coords {.x = 340, .y = 340};
  Point old_coords {.x = 340, .y = 340};
  Pixel color {.r = 0, .g = 0, .b = 254, .a = 255};
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H
