#include "Game.h"


void DrawLevel()
{
	ifstream file("levels/level1.txt");
  char line[400];
  int index = -1;

	Image      wall_tale("resources/wall.png");
	Image weak_wall_tale("resources/weak_wall.png");
	Image     floor_tale("resources/floor.png");
	Image      hole_tale("resources/hole.png");

	for (int i=0; i<80; ++i) {
		for (int j=0; j<80; ++j) {
			if (line[i] == '#') {
				this.PutImage(i*tileSize, j*tileSize, wall_tale);
			} else if (line[i] == '.' || line[i] == '@') {
				this.PutImage(i*tileSize, j*tileSize, floor_tale);
			} else if (line[i] == ' ') {
				this.PutImage(i*tileSize, j*tileSize, hole_tale);
			}
		}
	}
}