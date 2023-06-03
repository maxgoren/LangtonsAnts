#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

const int HEIGHT = 50;
const int WIDTH = 110;

typedef enum {
    BLACK,
    WHITE
} COLOR;

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} DIRECTION;

struct point {
    int x; 
    int y;
    point(int _x = 0, int _y = 0) {
        x = _x; y = _y;
    }
};


struct Tile : point {
    COLOR color;
    Tile(int _x = 0, int _y = 0, COLOR _color = WHITE) : point(_x, _y) {
        color = _color;
    }
};

#endif