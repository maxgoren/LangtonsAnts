#ifndef ANT_HPP
#define ANT_HPP
#include <cmath>
#include "typedefs.hpp"
using namespace std;


class Ant {
    private:
        point compass[4];
        DIRECTION direction;
        point location;
    public:
        Ant(int x = 0, int y = 0, DIRECTION dir = NORTH) {
            location.x = x; location.y = y;
            direction = dir;
            compass[NORTH] = point(0, -1);
            compass[SOUTH] = point(0, 1);
            compass[EAST] = point(1, 0);
            compass[WEST] = point(-1, 0);
        }
        void move(Tile grid[HEIGHT][WIDTH]) {
            if (grid[location.y][location.x].color == WHITE) {
                switch (direction) {
                    case NORTH: direction = EAST; break;
                    case SOUTH: direction = WEST; break;
                    case EAST: direction = SOUTH; break;
                    case WEST: direction = NORTH; break;
                }
                grid[location.y][location.x].color = BLACK;
            } else {
                switch(direction) {
                    case NORTH: direction = WEST; break;
                    case SOUTH: direction = EAST; break;
                    case EAST: direction = NORTH; break;
                    case WEST: direction = SOUTH; break;
                }
                grid[location.y][location.x].color = WHITE;
            }
            location.y = abs(((location.y + compass[direction].y) + HEIGHT) % HEIGHT);
            location.x = abs(((location.x + compass[direction].x) + WIDTH ) % WIDTH);
        }
        point& getLocation() {
            return location;
        }
};
