#include <ncurses.h>
void terminal_start();
void terminal_stop();
int make_color(int fg, int bg);
void colors_on(int fg, int bg);
void colors_off(int fg, int bg);
short curs_colors(int fg);
void init_colorpairs();

void terminal_start() {

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();
}

void terminal_stop() {

	endwin();
}

struct multiwins {
 WINDOW* win1;
 WINDOW* win2;
};

multiwins get_win_dims()
{
    multiwins retwins;
    int y, x;
    getmaxyx(stdscr, y, x);
    retwins.win1 = newwin(y,x/2-1,2,2);
    retwins.win2 = newwin(y,x/2-1,2, x/2+1);
    return retwins;
}

int make_color(int fg, int bg)
{
    //NCURSES recognizes 8 bits for colors
    //1 bit to turn blinking on and off
    //then 3 bits for background color
    //and four bits for fg (really 3 bits and a 0)
    int blink = 1 << 7; //set the furthest bit off
    int b = (7 & bg) << 4; //set the next 3 bits
    int f = 7 & fg; //last bits.
    //OR them all together and shake em all about
    return (blink | b | f); //thats what ncurses colors are about.
    //you can use this like:
    //attron(COLOR_PAIR(make_color(fg, bg))); to set colors :)
}

void colors_on(int fg, int bg)
{
    attron(COLOR_PAIR(make_color(fg, bg)));
}

void colors_off(int fg, int bg)
{
    attroff(COLOR_PAIR(make_color(fg, bg)));
}

short curs_color(int fg)
{
    switch (7 & fg) {           /* RGB */
    case 0:                     /* 000 */
        return (COLOR_BLACK);
    case 1:                     /* 001 */
        return (COLOR_BLUE);
    case 2:                     /* 010 */
        return (COLOR_GREEN);
    case 3:                     /* 011 */
        return (COLOR_CYAN);
    case 4:                     /* 100 */
        return (COLOR_RED);
    case 5:                     /* 101 */
        return (COLOR_MAGENTA);
    case 6:                     /* 110 */
        return (COLOR_YELLOW);
    case 7:                     /* 111 */
        return (COLOR_WHITE);
    }
}

void init_colorpairs()
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++) {
        for (fg = 0; fg <= 7; fg++) {
            colorpair = make_color(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
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