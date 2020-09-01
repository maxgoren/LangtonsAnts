/***************************************************************************************************
 *  Max Goren 8/29/2020
 * Langton's Ants - a cellular automata with very simple rules, that displays emergent behaviour.
 * At a white square, turn 90° clockwise, flip the color of the square, move forward one unit
 * At a black square, turn 90° counter-clockwise, flip the color of the square, move forward one unit
***************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <thread>
#include "ncurses-helpers.h"
using namespace std;

typedef uint32_t color_t; //0xAARRGGBB

/*************************************
 * Point represents Tiles on the board
 * **********************************/
struct Point {
    int x,y;   //coordinates
    char s;    //symbol - '#' = white, ' ' = black.
    color_t color; //0xFF000000 - black, 0xFFFFFFFF - white.
};

/***********************************
 * This class defines our Ant
 * *********************************/

class ant {
    int moves; //count the number of moves made
    int mapW;  // max width of the board
    int mapH;  // max heigh of the board.
    const color_t white = 0xFFFFFFFF;
    const color_t black = 0xFF000000;
    public:
    char facing;
    std::array<Point, 4> cards;
    Point pos;
    Point spos;
    Point fpos;
    vector<string> finalReport(Point fpos, WINDOW* win);
    void render(WINDOW* win);
    char updateFacing(bool cw);
    std::vector<std::vector<Point>> move(std::vector<std::vector<Point>> layout, WINDOW* win);
    ant(int w, int h, Point loc);
};

ant::ant(int w, int h, Point loc)
{
    cards[0] = {0,-1}; //N
    cards[1] = {0,1}; //S
    cards[2] = {-1,0}; //W
    cards[3] = {1,0}; //E
    this->facing = 'N'; //start facing north
    spos = pos = loc; //starting position
    moves = 0;
    mapW = w;
    mapH = h;
}

void ant::render(WINDOW* win)
{
    wattr_on(win, A_BOLD, NULL);
    wattr_on(win, COLOR_PAIR(make_color(4,3)), NULL);
    mvwaddch(win, pos.y, pos.x, '&');
    wattr_off(win, COLOR_PAIR(make_color(4,3)), NULL);
    wattr_off(win, A_BOLD, NULL);
}

std::vector<std::vector<Point>> ant::move(std::vector<std::vector<Point>> layout, WINDOW* win)
{
    char d; //for retrieving direction
    //get the color of where the ant is.
    Point nPos; //tile of the next position
    string msg;
    color_t currentColor = layout[pos.x][pos.y].color; //get the color of the tile the ant is on
    msg = "facing: ";
    msg.insert(msg.end()-1, facing);
    mvwaddstr(win,3,2,msg.c_str());
    msg = "location: "+to_string(pos.x) + "/" + to_string(pos.y);
    mvwaddstr(win,4,2,msg.c_str());
    if (currentColor == white)
    {
      msg = "Tile color: white";             //flip the color of the current tile
      mvwaddstr(win,5,2,msg.c_str());
      this->facing = d = updateFacing(true);                       //change direction as outlined in the rule set
      layout[pos.x][pos.y].color = black;
      layout[pos.x][pos.y].s = '#';
    } else { 
      msg ="Tile color: black";
      mvwaddstr(win,5,2,msg.c_str());
      this->facing = d = updateFacing(false);
      layout[pos.x][pos.y].color = white;
      layout[pos.x][pos.y].s = ' ';
    }
    
    facing = d;
    switch (d)                  //obtain the next tile based on the rule set.
    {
        case 'N': nPos = {pos.x + cards[0].x, pos.y + cards[0].y}; break;
        case 'S': nPos = {pos.x + cards[1].x, pos.y + cards[1].y}; break;
        case 'E': nPos = {pos.x + cards[3].x, pos.y + cards[3].y}; break;
        case 'W': nPos = {pos.x + cards[2].x, pos.y + cards[2].y}; break;
        default: break;
    }
    msg = "Turning:  to move to"+to_string(nPos.x) +"/"+to_string(nPos.y);
    msg.insert(msg.begin()+9, facing);
    mvwaddstr(win, 6, 2, msg.c_str());
    msg = "Move number: "+ to_string(moves);
    mvwaddstr(win,7,2,msg.c_str());
    if (nPos.x > 0 && nPos.x < mapW && nPos.y > 0 && nPos.y < mapH)
    {
        this->pos = nPos;   //if the next tile is not out of bounds, make it the current tile.
        moves++;
    } else {
        msg = "Out of bounds during move #: "+to_string(moves);
        mvwaddstr(win,7,2,msg.c_str());
        fpos = nPos;
        layout[1][1].s = 'X';        //if were out of bands, return to loop without changing position, layout[1][1].s = 'X'
    }                                //is used a flag to denote the ant's tour is over
    return layout;
}

char ant::updateFacing(bool cw)
{
    char cur = this->facing;
    if (cw)
    {
        if (cur == 'N') {
          this->facing = 'E'; 
          return facing;
        }
        if (cur == 'E') {
          this->facing = 'S'; 
          return facing;
        }
        if (cur == 'S') {
          this->facing = 'W'; 
          return facing;
        }
        if (cur == 'W') {
          this->facing = 'N'; 
          return facing;
        }
    }
    if (!cw)
    {
        if (cur == 'N') {
          this->facing = 'W'; 
          return facing;
        }
        if (cur == 'W') {
          this->facing = 'S'; 
          return facing;
        }
        if (cur == 'S') {
          this->facing = 'E'; 
          return facing;
        }
        if (cur == 'E') {
          this->facing = 'N'; 
          return facing;
        }
    }
    return facing;
}

vector<string> ant::finalReport(Point fpos, WINDOW* win)
{
    vector<string> report;
    string str;
    str = "Langton's Ant Final Report\n";   //create an array of strings
    report.push_back(str);                  //detailing the finishing state
    mvwaddstr(win, 8,2, str.c_str());                              // start position, # moves, 
    str = "Last Direction:  \n";            //where the ant stepped off the board
    str.insert(str.end() - 2, facing);
    report.push_back(str);
    mvwaddstr(win,9,2,str.c_str());
    str = to_string(moves) + " moves were made, starting from " + to_string(spos.x) + " / " + to_string(spos.y) + " on a 100x100 grid. \n";
    report.push_back(str);
    mvwaddstr(win,10,2,str.c_str());
    return report;
}

/*************************************************************
 * And now for the grid
************************************************************/

class antGrid {
    protected:
      vector<vector<Point>> layout;
      int mapW;
      int mapH;
    public:
    void render(ant antbro, WINDOW*);
    void loop(Point spos, multiwins winds);
    void cleanup(vector<string> report, WINDOW*);
    void saveOutput(vector<string> report);

    antGrid(int w, int h)
    {
        bool toggle = true;
        this->mapW = w; //set the size of the "board"
        this->mapH = h;
        layout.resize(w, vector<Point>(h)); //resize the vector to accomodate our layout size.
        int x,y;
        for (x = 0;x < w; x++)
        {
            for (y = 0; y < h; y++)
            {
                layout[x][y].color = 0xFF000000; //start off with a completely white board
                layout[x][y].s = '#';           //ascii version '#' is white and ' ' is black.
            }
        }
    }
       
};

void antGrid::loop(Point spos, multiwins winds)
{
    int k;
    int mode = 1; //1 = BLT, 2 = terminal ascii.
    start_color();
    init_colorpairs();  
    ant antbro(mapW, mapH, spos);
    while (true)
    {
        wclear(winds.win1);
        wclear(winds.win2);
        wattr_on(winds.win2, A_BOLD | A_UNDERLINE, NULL);
        mvwaddstr(winds.win2,2,5,"Langton's Ants");
        wattr_off(winds.win2, A_BOLD | A_UNDERLINE, NULL);
        this->layout = antbro.move(this->layout, winds.win2);
        if (layout[1][1].s == 'X')
        {
            cleanup(antbro.finalReport(antbro.fpos,winds.win2), winds.win2);
        }
        render(antbro, winds.win1);
        wborder(stdscr,0,0,0,0,0,0,0,0);
        wborder(winds.win1,0,0,0,0,0,0,0,0);
        wborder(winds.win2,0,0,0,0,0,0,0,0);
        wrefresh(winds.win1);
        wrefresh(winds.win2);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void antGrid::render(ant antbro, WINDOW* win)
{
    bool toggle = true;
    int x, y;
    int cx, cy;
    int sx, sy;
    sx = antbro.pos.x - 20;
    cx = antbro.pos.x + 20;
    sy = antbro.pos.y - 20;
    cy = antbro.pos.y + 20;
    attron(A_NORMAL);
    colors_on(3,3);
    for (x = 0; x < 80; x++)
    {
        
        for (y = 0; y < 40; y++)
        {
            if (layout[x][y].color == 0xFF000000)
                wattr_on(win, COLOR_PAIR(make_color(7,3)), NULL);
            if (layout[x][y].color == 0xFFFFFFFF)
                wattr_on(win, COLOR_PAIR(make_color(0,3)), NULL);
                mvwaddch(win, y, x, '#');
            if (layout[x][y].color == 0xFF000000)
                wattr_off(win, COLOR_PAIR(make_color(7,3)), NULL);
            if (layout[x][y].color == 0xFFFFFFFF)
                wattr_off(win, COLOR_PAIR(make_color(0,3)), NULL);
        }
    }
    antbro.render(win);
}


void antGrid::saveOutput(vector<string> report)
{
    int x, y;
    fstream antpic;
    antpic.open("langton.txt", ios::out|ios::trunc);
    if (antpic.is_open())
    {
        for (auto str : report)
          antpic << str;
        for (x = 0; x < mapW; x++)
        {
            for (y = 0; y < mapH; y++)
            {
                antpic << layout[x][y].s;
            }
            antpic << endl;
        }
        antpic.close();
    } else {
        cout<<"Error opening file for output.\n";
        antpic.close();
    }
}

void antGrid::cleanup(vector<string> report, WINDOW* win)
{
    saveOutput(report);
    mvwaddstr(win, 12,2,"Grid and stats saved to langton.txt");
    mvwaddstr(win,13,2,"Press any key to exit.");
    wrefresh(win);
    getch();
    endwin();
    exit(0);
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