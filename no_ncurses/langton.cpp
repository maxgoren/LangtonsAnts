#include <iostream>
#include "world.hpp"
using namespace std;


int main(int argc, char* argv[]) {
    World world(
        atoi(argv[1]), atoi(argv[2])
    );
    world.go();
    return 0;
}