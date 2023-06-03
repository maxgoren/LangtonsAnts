#ifndef WORLD_HPP
#define WORLD_HPP
#include <iostream>
#include <vector>
#include "typedefs.hpp"
#include "ant.hpp"
using namespace std;


class World {
    private:
        Tile grid[HEIGHT][WIDTH];
        vector<Ant> ants;
        int maxIter;
    public:
        World(int iters, int numants) {
            maxIter = iters;
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    grid[y][x].x = x;
                    grid[y][x].y = y;
                    grid[y][x].color = WHITE;
                }
            }
            DIRECTION dirs[4];
            dirs[0] = NORTH; dirs[1] = SOUTH; dirs[2] = EAST; dirs[3] = WEST;
            for (int i = 0; i < numants; i++) {
                ants.push_back(Ant(rand() % WIDTH, rand() % HEIGHT, dirs[rand() % 4]));
            }
        }
        void go() {
            for (int i = 0; i < maxIter; i++) {
                for (Ant & ant : ants) {
                    ant.move(grid);
                }
                show();
            }
        }
        void show() {
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    for (Ant ant : ants) {
                        if (x == ant.getLocation().x && y == ant.getLocation().y) {
                            cout<<"%";
                        }
                    }
                    if (grid[y][x].color == BLACK) {
                        cout<<".";
                    } else {
                        cout<<" ";
                    }
                }
                cout<<"\n";
            }
        }
};


#endif