#include "Player.h"
#include <string>
#include <fstream>
#include <iostream>
#include <ctime> 

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
  this-> wait = false;
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

  if (CheckFin(coords)) {
    Image win = Image("resources/win.png");
    screen.PutImage(0,0, win);
    std::clock_t t1, t2, t3;
 
    t1 = std::clock();
    t2 = std::clock();
    while (t2 - t1 < 1000000)
    {
      t2 = std::clock();
    }
    
     screen.PutImage(0,0, win);
     this->level_n++;
     InitLevel();
    
  }
  if (CheckHole(coords)) {
    //Image win = Image("resources/lose.png");
    //screen.PutImage(0,0, win);
    std::clock_t t1, t2, t3;
 
    t1 = std::clock();
    t2 = std::clock();
    while (t2 - t1 < 1000000)
    {
      t2 = std::clock();
    }
    
     //screen.PutImage(0,0, win);
     this->level_n--;
     InitLevel();
    
  }
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
  char c = lookup(pos);
  
  if (c == '.' || c == ' ' || c == 'x') {
    return true;
  } else {
    return false;
  }
}

bool Player::CheckFin(Point pos)
{
  if('x' == lookup({.x=coords.x + (*(this->heroSkin)).Height()/2, .y=coords.y + (*(this->heroSkin)).Width()/2}))
  {
    return true;
  } else {
    return false;
  }
}

bool Player::CheckHole(Point pos)
{
  if(' ' == lookup({.x=coords.x + (*(this->heroSkin)).Height()/2, .y=coords.y + (*(this->heroSkin)).Width()/2}))
  {
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


void Player::InitLevel()
{
  DrawLevel(*(this->level));
  this->coords.x = 340;
  this->coords.y = 340;
  this->old_coords.x = 340;
  this->old_coords.y = 340;

}