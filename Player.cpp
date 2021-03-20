#include "Player.h"
#include <string>
#include <fstream>
#include <iostream>

Player::Player (
    Point pos,
    const std::string &wall_tale_path,
    const std::string &weak_wall_tale_path,
    const std::string &floor_tale_path,
    const std::string &hole_tale_path,
    const std::string &finish_path,
    const std::string &heroSkin_path,
    int health
  )
{
  this->wall_tale      = new Image(wall_tale_path);
  this->weak_wall_tale = new Image(weak_wall_tale_path);
  this->floor_tale     = new Image(floor_tale_path);
  this->hole_tale      = new Image(hole_tale_path);
  this->finish_tale     = new Image(finish_path);
  this->heroSkin       = new Image(heroSkin_path);
  this->health = health;
  this->level = new Image(640, 640, 4);
  this->level_n = 1;
  DrawLevel(*(this->level));

}

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen)
{
  if(Moved())
  {
    if (CheckCoords()) {
      for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
      for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
          screen.PutPixel(x, y,  (*(this->level)).GetPixel(x,y));
      }}
      old_coords = coords;
    } else { // не можем пройти из-за припятствия
      coords = old_coords;
    }
  }
  screen.PutImage(0, 0, *(this->level));
  screen.PutImage(coords.x, coords.y, *(this->heroSkin));
}


void Player::DrawLevel(Image &screen)
{
  char buff[40];
  openCurrentLevelMap();
	for (int i=0; i<40; ++i) {
    this->levelinput.getline(buff, 40);
		for (int j=0; j<40; ++j) {
			if (buff[j] == '#') {
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->wall_tale));
			} else if (buff[j] == '.' || buff[j] == '@') {// todo
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->floor_tale));
			} else if (buff[j] == ' ') {
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->hole_tale));
			} else if (buff[j] == '*') {
        screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->weak_wall_tale));
      }else if (buff[j] == 'x') {
        screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->finish_tale));
      }
		}
  }
}

void Player::openCurrentLevelMap()
{
  this->levelinput.close();
  std::string genLevelPath = "levels/level"+std::to_string(this->level_n)+".txt";
  this->levelinput.open(genLevelPath);
}


bool Player::CheckCoords()
{
  bool res = true;
  res = (CheckPos({.x=coords.x, .y=coords.y})) 
        && (CheckPos({.x=coords.x + (*(this->heroSkin)).Height(), .y=coords.y + (*(this->heroSkin)).Width()}))
        && (CheckPos({.x=coords.x, .y=coords.y + (*(this->heroSkin)).Width()}))
        && (CheckPos({.x=coords.x + (*(this->heroSkin)).Height(), .y=coords.y}))
        && (CheckPos({.x=coords.x + (*(this->heroSkin)).Height(), .y=coords.y + (*(this->heroSkin)).Width()/2}))
        && (CheckPos({.x=coords.x + (*(this->heroSkin)).Height()/2, .y=coords.y + (*(this->heroSkin)).Width()}))
        && (CheckPos({.x=coords.x, .y=coords.y + (*(this->heroSkin)).Width()/2}))
        && (CheckPos({.x=coords.x + (*(this->heroSkin)).Height()/2, .y=coords.y}));
  return res;
}

bool Player::CheckPos(Point pos)
{
  this->levelinput.seekg(0);
  char buff[40];
  int x = pos.x,
      y = 640-pos.y;
  x = x/16;
  y = y/16;

	for (int i=0; i<=y; ++i) {
    this->levelinput.getline(buff, 40);
  }
  printf("%c,%i, %i \n", buff[x], x, y);
  if (buff[x] == '.' || buff[x] == ' ' || buff[x] == '?') {
    return true;
  } else {
    return false;
  }
}

char Player::lookup(Point pos)
{
this->levelinput.seekg(0);
  char buff[40];
  int x = pos.x,
      y = 640-pos.y;
  x = x/16;
  y = y/16;

	for (int i=0; i<=y; ++i) {
    this->levelinput.getline(buff, 40);
  }
  return buff[x];
}