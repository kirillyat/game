#include "Player.h"
#include <string>
#include <cstring>
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
    const std::string &heroSkin_path
  )
{
  this->wall_tale      = new Image(wall_tale_path);
  this->weak_wall_tale = new Image(weak_wall_tale_path);
  this->floor_tale     = new Image(floor_tale_path);
  this->hole_tale      = new Image(hole_tale_path);
  this->finish_tale     = new Image(finish_path);
  this->heroSkin       = new Image(heroSkin_path);
  this->health = 3;
  this->level = new Image(640, 640, 4);
  this->level_n = 1;
  openCurrentLevelMap();
  DrawLevel(*(this->level));
  this-> wait = false;
  this->lose_win_flag = false;
  
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
  if (this->lose_win_flag){
    return;
  }
  
 
  
  
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
    case MovementDir::ATTACK:
      break_weak_walls_around();
    break;
    default:
      break;
  }
}

void Player::Draw(Image &screen)
{
  if (this->lose_win_flag){
    if (this->health>0) {
      Image pic = Image("resources/win.png");
      screen.PutREVERSEDImage(10,10, pic);
    }else {
      Image pic = Image("resources/lose.png");
      screen.PutREVERSEDImage(10,10, pic);
    }
    return;
  }
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
    
     if (this->level_n < max_level)
     {
       this->level_n++;
       openCurrentLevelMap();
     } else {
      this->lose_win_flag = true;
      this->health = 3;
     }
     InitLevel();
    
  }
  if (CheckHole(coords)) {

       this->level_n-=1;
       this->health-=1;
       if ((this->health==0)||(this->level_n==0))
       { 
         this->lose_win_flag = true;
         this->health = 0;
       }
     InitLevel();
  }
  
}


void Player::DrawLevel(Image &screen)
{ 
	for (int i=0; i<40; ++i) {
		for (int j=0; j<40; ++j) {
			if (this->level_string[i][j] == '#') {
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->wall_tale));
			} else if (this->level_string[i][j] == '.' || this->level_string[i][j] == '@') {// todo
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->floor_tale));
			} else if (this->level_string[i][j] == ' ') {
				screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->hole_tale));
			} else if (this->level_string[i][j] == '*') {
        screen.PutImage(640-(j+1)*tileSize, i*tileSize, *(this->weak_wall_tale));
      }else if (this->level_string[i][j] == 'x') {
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
  this->levelinput.seekg(0);
  initLevelString();
  this->levelinput.seekg(0);
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
int x = pos.x,
      y = 640-pos.y;
  x = x/16;
  y = y/16;

	return this->level_string[y][x];
}
void Player::replace(Point pos, char distsyb)
{
  int x = pos.x,
      y = 640-pos.y;
  x = x/16;
  y = y/16;

	this->level_string[y][x] = distsyb;

}
void Player::replaceweakwall (Point pos)
{
  if ((pos.x < 1 || pos.x > 639)||(pos.y < 1 || pos.y > 639)) {
    return;
  }
  if (lookup(pos) == '*'){
    replace(pos, '.');
  }

}

void Player::InitLevel()
{
  DrawLevel(*(this->level));
  this->coords.x = 340;
  this->coords.y = 340;
  this->old_coords.x = 340;
  this->old_coords.y = 340;

}

void Player::break_weak_walls_around()
{
  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height() +8, .y=coords.y});
  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height(), .y=coords.y -8});
  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height() +8, .y=coords.y-8});

  replaceweakwall({.x=coords.x -8, .y=coords.y+(*(this->heroSkin)).Height()});
  replaceweakwall({.x=coords.x , .y=coords.y +8+(*(this->heroSkin)).Height()});
  replaceweakwall({.x=coords.x -8, .y=coords.y+8+(*(this->heroSkin)).Height()});

  replaceweakwall({.x=coords.x -8, .y=coords.y});
  replaceweakwall({.x=coords.x , .y=coords.y -8});
  replaceweakwall({.x=coords.x -8, .y=coords.y-8});

  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height() +8, .y=coords.y+(*(this->heroSkin)).Height()});
  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height(), .y=coords.y +8+(*(this->heroSkin)).Height()});
  replaceweakwall({.x=coords.x + (*(this->heroSkin)).Height() +8, .y=coords.y+8+(*(this->heroSkin)).Height()});

  replaceweakwall({.x=coords.x+(*(this->heroSkin)).Height()/2, .y=coords.y-8});
  replaceweakwall({.x=coords.x +(*(this->heroSkin)).Height()/2, .y=coords.y+(*(this->heroSkin)).Height()+8 });
  replaceweakwall({.x=coords.x -8, .y=coords.y+(*(this->heroSkin)).Height()/2});
  replaceweakwall({.x=coords.x +(*(this->heroSkin)).Height()/2 +8, .y=coords.y +(*(this->heroSkin)).Height()});


  DrawLevel(*(this->level));
}

void Player::initLevelString()
{

	for (int i=0; i<40; ++i) {
    this->levelinput.getline(this->level_string[i], 40);
	}
}