/*******************
 * usage: 
 * ./ants x y
 * 
 * ****************/

#include "ants.h"
#include <signal.h>
void resizeHandler(int sig);
int main(int argc, char *argv[])
{
    
    int x, y;
    if (argc < 2)
    {
        cerr <<"Please suplly a starting coordinate as "<<argv[0]<<" x y \n";
        return -1;
    }
    multiwins winds;
    initscr();
    getmaxyx(stdscr, y, x);
    winds = get_win_dims();
    Point spos = {atoi(argv[1]), atoi(argv[2])};
    antGrid AntLife(200, 200);  //<---------------change these values to change grid size.
    signal(SIGWINCH, resizeHandler);
    AntLife.loop(spos, winds);
    exit(0);
    return 0;
}

void resizeHandler(int sig)
{
    int nh, nw;
    terminal_stop();
    terminal_start();
}

/*
Copyright (c) 2020 Max Goren
maxgoren@icloud.com
http://www.maxcodes.info

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/