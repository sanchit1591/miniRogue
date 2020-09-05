//
//  Actor.hpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/20/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef Actor_h
#define Actor_h

#include "globals.h"
#include "utilities.h"
#include "GameObject.h"
#include <queue>
//as dungeon pointer is present in the Player

///////////////////////////////////////////////////////////////
//STRUCTURE FOR GOBLIN MOVE
///////////////////////////////////////////////////////////////

struct spotData
{
    int row;
    int col;
    int distance;
};

const int m_pp[5] = {0,0,0,0,0};
class Dungeon;

///////////////////////////////////////////////////////////////
//ACTOR CLASS   <<<SUPER CLASS>
///////////////////////////////////////////////////////////////

//Base Class
//Contains attributes that are common to both Player and Monsters
//Derived Classes -> Player , Monster

//COMMONALITIES OF ALL ACTORS
/*
    1)They all lie in some Dungeon
    2)They all have a position
    3)They all have four types of Hitpoints
    4)They all have a sleep time
    5)They all have a symbol
    6)they all have a weapon
    7)They all have a name
 
    Therefore, All the things must be stored in the actors class as they are common
*/

class Actor
{
    public:
    //Actor Constructor --> initializes all the commonalities
        Actor(Dungeon *dungeon , Position position , Weapon *weapon , string name , char symbol , int PointArray [])
        {
            m_dungeon = dungeon;
            m_pos = position;
            m_weapon = weapon;
            m_name = name;
            m_symbol = symbol;
            m_pointArray = PointArray;
            m_dead = false;
        }
    
        Actor(Weapon *weapon , string name , char symbol , int PointArray [])
        {
            m_weapon = weapon;
            m_name = name;
            m_symbol = symbol;
            m_pointArray = PointArray;
            m_dead = false;
        }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //ACCESSORS
    /////////////////////////////////////////////////////////////////////////////////////////////////////
        
        //GET GRID STATUS
        Position getPos() const {   return m_pos;   };
        bool isDead() const { return m_dead;  }
        
        //GET POINTS
        int getHitpoints() const {  return m_pointArray[0]; }
        int getArmorPoints() const {  return m_pointArray[1]; }
        int getStrengthPoints() const {   return m_pointArray[2]; }
        int getDexterityPoints() const {  return m_pointArray[3]; }
        int getSleepTime() const {   return m_pointArray[4]; }
    
        //GET REFERENCES
        
        Weapon *getWeapon() const { return m_weapon;  }
        Dungeon* getdungeon() {   return m_dungeon;   }
    
        //OTHER GETTERS
        char getSymbol() const {  return m_symbol;  }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //MUTATORS
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
        //UPDATE POINTS
        
        //Inline Definitions
        //Hit points updated by the given value
        //Use global constants -> ADD(true) and SUBTRACT(false) in the update parameter
        void updateHitPoints(bool update , int value) {m_pointArray[0] += (update)?value:(-value);}
        void updateArmorPoints(bool update , int value) {m_pointArray[1] += (update)?value:(-value);}
        void updateStrengthPoints(bool update , int value) {m_pointArray[2] += (update)?value:(-value);}
        void updateDexterityPoints(bool update , int value) {m_pointArray[3] += (update)?value:(-value);}
        void updateSleepTime(bool update , int value) {m_pointArray[4] += (update)?value:(-value);}
    
        //OTHER MUTATORS
        void setPos(Position pos) {     m_pos = pos;    }
        void setWeapon(Weapon *weapon) {    m_weapon = weapon;  }
        void setDead() {    m_dead = true;  }
    
        //For Player
        void setPosition(Position pos)  {   m_pos = pos;    }   //Sets the player's position
        void setDungeon(Dungeon* d) {  m_dungeon = d;   }   //Sets the data member m_dungeon
        
    /////////////////////
    //OTHER IMPORTANT FUNCTIONS
    ////////////////////
        string attack(Actor* attacker , Actor* defender);
    
        virtual ~Actor()
        {
            delete m_weapon;
        }
    
    private:
        
        /*
         0  --> HIT POINTS
         1  --> ARMOR POINTS
         2  --> STRENGTH POINTS
         3  --> DEXTERITY POINTS
         4  --> SLLEP TIME
         */
        Dungeon *m_dungeon;
        Weapon *m_weapon;
    
        Position m_pos;
        int *m_pointArray;
        char m_symbol;
        string m_name;
    
    
        bool m_dead;
        
};


///////////////////////////////////////////////////////////////
//PLAYER CLASS
///////////////////////////////////////////////////////////////


//Derived Class from Actor
//Defines the player

class Player : public Actor
{
    public:
        Player() : Actor(new ShortSword , "Player " , '@' , m_points)
        {
            m_maxhitPoints = 20;
            inventory.push_back(getWeapon());
            m_won = false;
        }
        
        //PLAYER ACTIONS
        bool attemptmove(char direction , Position &mon_pos);  //passing parameters to know which monster is to be attacked
        string move(char direction);
        string pickObject();
        string weildWeapon();
        string readScroll();
        void displayInventory();
        void cheat();
    
        //Other functions
        //TODO:CHECK!!!!!!!!!!!!!!!!!!!!
//        void setPosition(Position pos)  {   m_pos = pos;    }   //Sets the player's position
//        void setDungeon(Dungeon* d) {  m_dungeon = d;   }   //Sets the data member m_dungeon
//        virtual Position getPos() const {   return m_pos;   };
//        virtual Dungeon* getdungeon() const {   return m_dungeon;   }
        
    
        //Inline Definitions
    
    
        //MaxHitPoints Related Functions
        //Only addition in Max Hit Points (by 1/10 prob or cheat)
        void increaseMaxHitPoints(int update_value) {   m_maxhitPoints += update_value; }
        int getMaxHitPoints() const {   return m_maxhitPoints;   }
    
        //Inventory
        bool isInventoryfull() {    return inventory.size() == MAX_PLAYER_INVENTORY;    }
        bool didPlayerWin() {   return m_won;   }
    
        virtual ~Player()
        {
            for(int i = 0; i < inventory.size() ; i++)
            {
                if(inventory[i] != getWeapon())
                {
                    delete inventory[i];
//                    inventory.erase(inventory.begin() + i);
                }
            }
        };

    
    private:
        vector<GameObject*> inventory;
        int m_points[5] = {20,2,2,2,0};
//        Dungeon* m_dungeon = nullptr;
//        Position m_pos;
        
        int m_maxhitPoints;
        bool m_won;
        
    
};


///////////////////////////////////////////////////////////////
//MONSTER CLASS
///////////////////////////////////////////////////////////////

//Sub Base class for different types of monsters
//Derived Class from Actor
//Defines the commonalities of all the Monsters

class Monster : public Actor
{
    public:
    
        //CONSTRUCTOR....
        Monster(Dungeon *dungeon , Position position , Weapon *weapon , string name , char symbol , int PointArray [] , int smell = 0) : Actor(dungeon,position,weapon,name,symbol,PointArray)
        {
            m_smell = smell;
        }
        void setGoblinSmell(int smell) {    m_Gob_smell = smell;   }
        int getGoblinSmell()   {   return m_Gob_smell; }
//    //functions that aid movement
    
        bool isPlayerinRange(int steps , Position mon_pos); //Checks if player is in range of a Monster -->Bogeymen,Snakewomen smell function
        int distancefromPlayer(Position p_pos,Position m_pos);  //applies pythagores theorem to find the distance of a player
    
        bool attemptmove(char direction , int& distance);   //Attempts to move in a particular direction the result of this aids monster move selection
    //movement of Monsters
    //programmed according to BogeyMan and snakewoman , dragon and Goblin override
        virtual string move();
    
    virtual ~Monster() {};
    private:
        int m_Gob_smell = 0;
        int m_smell = 0;
};


///////////////////////////////////////////////////////////////
//SNAKEWOMEN CLASS
///////////////////////////////////////////////////////////////

//Derived Class from Monster
//Defines the Snakewoman

class Snakewoman : public Monster
{
    public:
        Snakewoman(Dungeon *m_d , Position position) : Monster(m_d , position ,new MagicFangs , "the Snakewoman ", 'S', m_points , 3) {};
    
    private:
        
        int m_points[5] = {randInt(3, 6) , 3 , 2 , 3 , 0};
    
};


///////////////////////////////////////////////////////////////
//GOBLIN CLASS
///////////////////////////////////////////////////////////////

//Derived Class from Monster
//Defines the Goblin

class Goblin : public Monster
{
    public:
        Goblin(Dungeon *m_d , Position position) : Monster(m_d , position ,new ShortSword , "the Goblin ", 'G', m_points) {};
    
//    int pQuadrant(Position player,Position goblin);
//    bool directionArr(int arr [], Position player , Position goblin , int smell);
    int pathExists(Position player,int max_smell);
    bool movableSpot(int row,int col);
    void clearData();
    virtual string move();  //override
    
    private:
        queue<spotData> data;
        int m_points[5] = {randInt(15, 20) , 1 , 3 , 1 , 0};
        vector<int> steps;
};


///////////////////////////////////////////////////////////////
//BOGEYMAN CLASS
///////////////////////////////////////////////////////////////

//Derived Class from Monster
//Defines the Bogeyman

class Bogeyman : public Monster
{
    public:
        Bogeyman(Dungeon *m_d , Position position) : Monster(m_d , position ,new ShortSword , "the Bogeyman ", 'B', m_points, 5) {};
    
    private:
        
        int m_points[5] = {randInt(5, 10) , 2 , randInt(2, 3) , randInt(2, 3) , 0};
};



///////////////////////////////////////////////////////////////
//DRAGON CLASS
///////////////////////////////////////////////////////////////

//Derived Class from Monster
//Defines the Dragon

class Dragon : public Monster
{
    public:
        Dragon(Dungeon *m_d , Position position) : Monster(m_d , position ,new LongSword , "the Dragon ", 'D', m_points) {};
        virtual string move();  //override
    private:
        
        int m_points[5] = {randInt(20, 20) , 4 , 4 , 4 , 0};
};


#endif
