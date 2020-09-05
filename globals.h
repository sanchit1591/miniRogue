//
//  globals.h
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/18/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef globals_h
#define globals_h

/////////////////////////////////////////////////////////////////////////////////
//SOME INCLUDES --> For easy access in other files
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
//DUNGEON CONSTANTS
/////////////////////////////////////////////////////////////////////////////////

//DUNGEON SIZE
const int MAX_D_ROWS = 18;
const int MAX_D_COLUMNS = 70;

//ROOM PROPERTIES
const int MIN_ROOM_LENGTH = 3;
const int MIN_ROOM_WIDTH = 8;
const int MAX_ROOM_LENGTH = 8;
const int MAX_ROOM_WIDTH = 15;

//OPTIONS OF ROOMS
    //used when printing rooms more than four
const int UP = 1;
const int DOWN = -1;

/////////////////////////////////////////////////////////////////////////////////
//PRINATABLE CHARACTERS
/////////////////////////////////////////////////////////////////////////////////
const char WALL = '#';
const char PATH = ' ';
const char PLAYER = '@';
const char SNAKEWOMEN = 'S';
const char BOGEYMEN = 'B';
const char GOBLIN = 'G' ;
const char DRAGON = 'D' ;
const char WEAPONS = ')' ;
const char SCROLLS = '?' ;

/////////////////////////////////////////////////////////////////////////////////
//PLAYER MOVES
/////////////////////////////////////////////////////////////////////////////////
const char PICKUP = 'g';
const char INVENTORY = 'i';
const char WEILD = 'w';
const char READ = 'r';
const char CHEAT = 'c';
const char STAIRS = '>';
const char IDOL = '&';


//To update various actor points ...
const bool ADD = true;
const bool SUBTRACT = false;
const int MAX_PLAYER_INVENTORY = 26;
const int CHEAT_HIT_POINTS = 50;
const int CHEAT_STRENGTH_POINTS = 9;
const int MAX_LEVELS = 4;

/////////////////////////////////////////////////////////////////////////////////
//GOBLIN MOVES
/////////////////////////////////////////////////////////////////////////////////
const char VISITED = '*';
const int QUADRANT_ONE = 1;
const int QUADRANT_TWO = 2;
const int QUADRANT_THREE = 3;
const int QUADRANT_FOUR = 4;
const int POSITIVE_X = 5;
const int POSITIVE_Y = 6;
const int NEGATIVE_X = 7;
const int NEGATIVE_Y = 8;
const int ORIGIN = 0;
const int N = 0;
const int S = 1;
const int E = 2;
const int W = 3;
const int NE = 4;
const int NW = 5;
const int SE = 6;
const int SW = 7;
const int O = 8;

/////////////////////////////////////////////////////////////////////////////////
//GLOBAL CLASSES
/////////////////////////////////////////////////////////////////////////////////
struct Position
{
    //DEFAULT CONSTRUCTOR
    Position() {};
    
    //CONSTRUCTOR WITH ROW AND COLUMN ARGUMENTS
    //inline definition
    Position(int t_row , int t_col) : row(t_row),col(t_col) {};
    
    void set(int r , int c)
    {
        row = r;
        col = c;
    }
    int row;
    int col;
};
//
//bool operator==(const Position &p1 , const Position &p2)
//{
//    return (p1.row == p2.row && p1.col == p2.col);
//}
//

#endif /* globals_h */
