//
//  Game.hpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/18/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "globals.h"
#include "Actor.h"
#include "Dungeon.h"

///////////////////////////////////////////////////////////////
//GAME CLASS
///////////////////////////////////////////////////////////////

//Handles the entire game
//Creates a dungeon and handles all moves


class Game
{
    public:
    
    //CONSTRUCTOR
        Game(int goblinSmellDistance);
    
    //ALL PLAYER COMMANDS ARE PROCESSED IN THIS FUNCTION
        string takePlayerTurn(char playerMove);
    
    //INLINE --> RETURNS IF GAME IS OVER OR NOT
        bool GameOver() { return m_player->isDead() || plyr_wins;}
    
    //UPDATES THE LEVEL
        void updateLevel();
    
    //THE PLAY FUNCTION WHICH TAKES CARE OF ALL THE MOVING
        void play();
    
    //DESTRUCTOR
        ~Game();
        
    private:
        Dungeon *m_dungeon;
        Player *m_player;
        bool m_over;
        bool plyr_wins;
        int m_level;
        int m_goblinSmell;
};


#endif /* Game_h */
