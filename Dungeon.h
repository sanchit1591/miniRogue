//
//  Dungeon.hpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/18/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include "globals.h"
#include "Actor.h"
#include "GameObject.h"

///////////////////////////////////////////////////////////////
//RoomSTRUCTURE
///////////////////////////////////////////////////////////////


//  a simple structure used when rooms are created
//  stores the boundaries of the room
//  Used Where? -> Only used in the Dungeon that's why only included here

struct Room
{
    int top_row;
    int bot_row;
    int left_column;
    int right_column;
    int pos;
    int data1;
};



///////////////////////////////////////////////////////////////
//DUNGEON CLASS
///////////////////////////////////////////////////////////////

class Dungeon
{
    public :
    
        //DUNGEON CONSTRUCTOR
            //Called in Game
            //Takes two parameters for appropriate setting
                //level --> determines number of Monsters , Used in Status Bar
                //Player ptr --> passed as a parameter as Dungeon is deleted after each level
    
        Dungeon(int level , Player *Player);
    
        //POPULATE DUNGEON
        void addPlayer();
        void addMonsters(int goblinSmelldistance);
        void addGameObjects();
        
        ///////////////////////////////////////////////////////////////////////
        //ACCESSORS
        ///////////////////////////////////////////////////////////////////////
    
        ////////////////////////////
        //CHECKING SPOTS IN THE GRID
        ////////////////////////////
    
        bool isPlayerAt(Position pos) const;  
    
        //The second parameter is to get
        //which Object/Monster in the GameObject/Monster Vector
        //is present on the particular spot
        bool isObjectAt(Position pos , int& which) const;
        bool isMonsterAt(Position pos , int& which) const;
        
        //Calls two other functions ...Just for clearity of Code
        //Anything can be easily moved to this spot
        bool isEligibleSpot(Position pos) const;
    
        bool isInGrid(Position pos) const;
        bool isWallAt(Position pos) const { return m_grid[pos.row][pos.col] == WALL;}
        bool isStaircaseAt(Position pos) const ;
        bool isIdolAt(Position pos) const;
        bool isVisitedbyGoblin(Position pos) const {    return m_grid[pos.row][pos.col] == VISITED;     }
        void unMarkVisited();
    
        Position getEligiblePosition();
        
    
        ////////////////////////////
        //OBJECT GETTERS
        ////////////////////////////
    
    
        //inline definitions
        Player *getPlayer() const   {   return m_player;    }
    
        //These two functions will be used always in sync with isObjectAt and isMonsterAt
        Monster *getMonster(int which) const  {   return m_Monster_vector[which];   }
        GameObject *getObject(int which) const  {   return m_GameObject_vector[which]; }
    
        
        ////////////////////////////
        //DISPLAY
        ////////////////////////////
    
        void display();
    
        

        ///////////////////////////////////////////////////////////////
        //MUTATORS
        ////////////////////////////////////////////////////////////////
    
        /////MOVE AND KILL MONSTERS
        string moveMonsters();
        void killMonster();
        void markVisitedbyGoblin(Position pos)  {  m_grid[pos.row][pos.col] = VISITED;  }
    
    
        //Used when Player Picks up and Object
        void deleteObjectfromGrid(int index);
    
        ////////////////////////////
        //ROOMS
        ////////////////////////////
    
    
        void CreateRooms();
        
    
        ~Dungeon();
    
    private :
    ////////////
    //BASIC MEMBERS -> REQUIRED IN CONSTRUCTION OF DUNGEON
    ////////////
    
        //Character grid that represents the game
        char m_grid[MAX_D_ROWS][MAX_D_COLUMNS];
        int m_level;
    
    ////////////
    //OBJECT CONTAINERS -> CONTAINS THE PLAYER,MONSTERS ,GAMEOBJECTS
    ////////////
    
        Player *m_player;
        vector<Monster*> m_Monster_vector;
        vector<GameObject*> m_GameObject_vector;
        Staircase *m_stairs;
        GoldenIdol *m_idol;
    
    ///////////
    //ROOM CREATION
    ///////////
    
        vector<Room> m_rooms_vector;
    
    ///////////
    //PRIVATE FUNCTIONS
    ///////////
    
    ////////////////////////////
    //SOME BOOLEANS
    ////////////////////////////
            
    //Made to use common function even when the player isn't placed on the board
    //Used for placing the player on the board
        bool isPlayerplaced;
//        bool isDead;
    //bool areMonstersplaced;
    
    
    //A function that cout's the Player Status Bar
    //Called by Display only
        void getStatusBar();
    
    
    //A function called by CreateRooms only
    //Purpose - > Aids the process of Path Making
        void Mark_Room(int row,int col,int length,int width,int marker);
    
    
};




#endif /* Dungeon_h */
