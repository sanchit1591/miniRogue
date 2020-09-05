//
//  Game.cpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/18/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//
#include "Game.h"
#include "utilities.h"
#include "Dungeon.h"
#include "Actor.h"
#include <iostream>
using namespace std;

// Constructor calls update level which is adapt to deal with Creation of game at the first point

Game::Game(int goblinSmellDistance)
{
    m_goblinSmell = goblinSmellDistance;
    plyr_wins = false;
    
    //Level initiated to -1 , as the moment it enters the updateLevel function it is updated and starts off with zero
    m_level = -1;
    

    updateLevel();
}

string Game::takePlayerTurn(char playerMove)
{
//1.    set up player move string
    string msg ="";

   
//2.    if sleep time isn't 0 , subtract one and return empty string
    if(m_player->getSleepTime() != 0)
    {
        m_player->updateSleepTime(SUBTRACT, 1);
        return msg;
    }
    
    
//3.    Update hitpoints by a probability of 0.1
    bool hitPointsUpdate_prob = trueWithProbability(0.1);
    
    if(hitPointsUpdate_prob == true && m_player->getHitpoints() < m_player->getMaxHitPoints())
    {
        m_player->updateHitPoints(ADD, 1);
    }
    
    
//4.    Call various functions of player's class according to the user command
//AS I HAVE MADE GLOBAL VARIABLES WHICH MAKE IT PRETTY CLEAR AS TO WHAT IS HAPPENING , I DIDN'T FEEL THE NEED TO ADD COMMENTS HERE 
    switch(playerMove)
    {
        case ARROW_UP :
        case ARROW_DOWN :
        case ARROW_LEFT :
        case ARROW_RIGHT :
            msg += m_player->move(playerMove);
            break;
        case PICKUP :
            msg += m_player->pickObject();
            
            //    if player is on the golden idol and presses 'g' . set the palyer to be winner
            if(m_player->didPlayerWin() == true)
            {
                plyr_wins = true;
            }
            break;
        case INVENTORY :
            m_player->displayInventory();
            getCharacter();                //UNDERSTAND GET_CHARACTER AND CHANGE!!!!!!!!!!!!!!
            break;
        case WEILD :
            m_player->displayInventory();
            msg += m_player->weildWeapon();
            cout<<endl;
            break;
        case READ :
            m_player->displayInventory();
            msg += m_player->readScroll();
            break;
        case CHEAT :
            m_player->cheat();
            break;
            
        //    if the player is on staircase and presses '>'  then update level
        case STAIRS :
            if(m_dungeon->isStaircaseAt(m_player->getPos()))
            {
                updateLevel();
            }
            break;
        default :
            break;
    }
    return msg;
}
void Game::play()
{
    char temp;
    string plyr_move;
    string monster_move;
    
    //1.    While user dosen't inputs 'q' or Game is Over
    while (temp != 'q'  &&  GameOver() == false)
    {
        
        //    getCharacter from the player
        temp = getCharacter();
        cout<<endl;
        
        //    call the function takePlayerTurn and pass that character
        plyr_move = takePlayerTurn(temp);
        
        //    if player wins cout player move and break from the loop
        if(plyr_wins == true)
        {
            cout<<plyr_move;
            break;
        }
        //    Move monsters
        monster_move = m_dungeon->moveMonsters();
        clearScreen();
        m_dungeon->display();
        cout<<endl<<plyr_move<<monster_move;
    }
    
    //CHECK EXIT CONDITION
    
//        if player wins or is dead
//        while player dosen't enter q -> cout the exit message,clearscreen and display dungeon ..

    if(plyr_wins == true || m_player->isDead())
    {
        cout << "Press q to exit game." << endl;
        while(temp != 'q')
        {
            temp = getCharacter();
            if(temp != 'q')
            {
                clearScreen();
                m_dungeon->display();
                cout << "Press q to exit game." << endl;
            }
        }
    }
}

void Game::updateLevel()
{
    //Generalizing it to accomodate zero level also
    //starting level count from -1
    //updating it before passing
    
    //1.    if level is greater than or equal to 0 delete current dungeon
    if(m_level >= 0)
    {
        delete m_dungeon;
    }
    
    //2.    if level is equal to -1 create a new player
    if(m_level < 0)
    {
        m_player = new Player;
    }
    
    //3.    update level private member value
    m_level++;
    
    //4.    create a new dungeon and pass level, player pointer as parameters
    m_dungeon = new Dungeon(m_level, m_player);
    
    //5.    add game objects to dungeon
    m_dungeon->addGameObjects();
    
    //6.    add Player to the dungeon
    m_dungeon->addPlayer();
    
    //7.    add Monsters to the dungeon(pass goblin smell as parameter)
    m_dungeon->addMonsters(m_goblinSmell);
    m_over = false;
    plyr_wins = false;
    
    //8.    if level equals 0 , display the dungeon
    if(m_level == 0)
        m_dungeon->display();
}


Game::~Game()
{
    delete m_dungeon;
    delete m_player;
}
