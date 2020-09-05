//
//  Dungeon.cpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/18/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#include "Dungeon.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//DUNGEON CONSTRUCTION AND DISPLAY///////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


Dungeon::Dungeon(int level , Player* player)
{
    //Initially create the entire dungeon covered by Walls
    //Then create rooms
    for(int r_i = 0; r_i < MAX_D_ROWS ; r_i++)
        for(int c_i = 0; c_i < MAX_D_COLUMNS ; c_i++)
            m_grid[r_i][c_i] = WALL;
    
    //Create Rooms
    CreateRooms();
    m_level = level;
    m_player = player;
    isPlayerplaced = false;
//    isDead = false;
}

void Dungeon::display()
{
    Position check;
    int which;
    
    //move over each spot in the grid
    //(printing items are arranged according to the precedence where gameObjects are placed below players and monster because of which if a particular position is occupied by both then the Actor will be shown
    for(int r_i = 0; r_i < MAX_D_ROWS ; r_i++)
    {
        for(int c_i = 0; c_i < MAX_D_COLUMNS ; c_i++)
        {
            check.set(r_i, c_i);
            
            if(isWallAt(check))
                    cout << WALL;
            
            else if(isPlayerAt(check))
            {
                cout << m_player->getSymbol();
            }
            
            else if(isMonsterAt(check,which))
            {
                cout << m_Monster_vector[which]->getSymbol();
            }

            else if(isObjectAt(check,which))
            {
                cout << m_GameObject_vector[which]->getSymbol();
            }
            
            else if(isStaircaseAt(check))
            {
                cout << m_stairs->getSymbol();
            }
            
            else if(isIdolAt(check))
            {
                cout << m_idol->getSymbol();
            }
    
            else
                cout << PATH ;
        }
        cout<<endl;
    }
    getStatusBar();
    
}

//Prints the Status Bar
void Dungeon::getStatusBar()
{
    cout<<"Dungeon Level: "<<m_level;
    cout<<", Hit points: "<<getPlayer()->getHitpoints();
    cout<<", Armor: "<<getPlayer()->getArmorPoints();
    cout<<", Strength: "<<getPlayer()->getStrengthPoints();
    cout<<", Dexterity: "<<getPlayer()->getDexterityPoints()<<endl;
    
}


void Dungeon::unMarkVisited()
{
    for(int r_i = 0; r_i < MAX_D_ROWS ; r_i++)
    {
        for(int c_i = 0; c_i < MAX_D_COLUMNS ; c_i++)
        {
            if(isVisitedbyGoblin(Position(r_i, c_i)))
                m_grid[r_i][c_i] = PATH;
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//ROOM MAKING IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////





/*
 The function below create rooms and Paths between them
 The number of rooms range from 3 to 6 with the probability of 6 being less than 4
 The rooms happen to be completely random
 The Path making algorithm ensures a completely random nature and there is no fixed or predefined point connection for making the Paths
 */

void Dungeon::CreateRooms()
 {
    int num_rooms = randInt(3, 4);
    
    vector<int> Partitions;
    
     //Decide the appropriate threshold for room sizes
    int fair_partition = (MAX_D_COLUMNS - 2)/num_rooms;
    int margins = 0;
    
    //Divide the grid in 3 - 4 sections of Random length within given threshold vertically
    for(int k = 0 ; k < num_rooms - 1 ; k++ )
    {
        Partitions.push_back(margins + randInt(fair_partition - 6, fair_partition + 2));
        margins = Partitions[k];
    }

    //Various room parameters decided in the loop below
    int row = 0;
    int row2 = 0;
    int col = 0;
    int col2 = 0;
    int length = 0;
    int length2 = 0;
    int width = 0;
    int width2 = 0;
     
    vector<Room> data;
    vector<Room> data2;
    Room my;
    Room my2;
     
     //OVERLAPS IS BASICALLY A ROOM WHICH OCCUPIES A SECTION ALREADY OCCUPIED BY ANOTHER ROOM
     bool make_overlaps = false;
     int s1 = randInt(0, num_rooms - 1);
     int s2 = randInt(0, num_rooms - 1);
     while(s1 == s2)
     {
         s2 = randInt(0, num_rooms - 1);
     }
     
    //AS formation of 5-6 rooms is rare I limit it to two partitions and that too with the probablility geiven below
    bool sixprob = trueWithProbability(0.7);
     
    //DRAWS LINEAR ROOMS
     /*
      this will just allocate the rooms not drawing paths yet
      */
    for(int i = 0; i < num_rooms; i++)
    {
        //For each Partition select a row and a length whithin a given threshold
        row = randInt(1, 8);
        my.top_row = row;
            
        length = randInt(MIN_ROOM_LENGTH, MAX_ROOM_LENGTH);
        my.bot_row = row + length - 1;
        
        
        //MAKING OVERLAPS
        //OVERLAP ABOVE
        if(my.top_row >= 7 && (i == s1 || i  == s2) && sixprob)
        {
            row2 = randInt(1,2);
            my2.top_row = row2;
            length2 = 3;
            my2.bot_row = my2.top_row + 2;
            my2.pos = UP;
            make_overlaps = true;
            my2.data1 = i;
        }
        
        //OVERLAP BELOW
        if(my.bot_row <= 9 && (i == s1 || i  == s2) && sixprob)
        {
            length2 = randInt(3, 5);
            row2 = randInt(11, 16 - length2);
            my2.top_row = row2;
            my2.bot_row = my2.top_row + length2 - 1;
            my2.pos = DOWN;
            make_overlaps = true;
            my2.data1 = i;
        }
        
        //Find the width and col within the given threshold for the "linear" room and "overlap" room and then push them into 2 different vectors as "Room" structure
        if(i == 0)
        {
            //NORMAL ROOM
            width = randInt(8, Partitions[i] - 2);
            col = randInt(1, Partitions[i] - width);
            
            //OVERLAP
            width2 = randInt(8, Partitions[i] - 2);
            col2 = randInt(1, Partitions[i] - width2);
            my2.left_column = col2;
            my2.right_column = col2 + width2 - 1;
        }
        if(i == num_rooms - 1)
        {
            //NORMAL ROOM
            width = randInt(8, min(MAX_D_COLUMNS - Partitions[i - 1] - 4,MAX_ROOM_WIDTH));
            col = randInt(Partitions[i - 1] + 2, MAX_D_COLUMNS - 2 - width);
            
            //OVERLAP
            width2 = randInt(8, min(MAX_D_COLUMNS - Partitions[i - 1] - 4,MAX_ROOM_WIDTH));
            col2 = randInt(Partitions[i - 1] + 2, MAX_D_COLUMNS - 2 - width2);
            my2.left_column = col2;
            my2.right_column = col2 + width2 - 1;
        }
        if(i > 0 && i < num_rooms - 1)
        {
            //NORMAL ROOM
            width = randInt(8, Partitions[i] - Partitions[i - 1] - 2);
            col = randInt(Partitions[i-1] + 2,Partitions[i] - width);
            
            //OVERLAP
            width2 = randInt(8, Partitions[i] - Partitions[i - 1] - 2);
            col2 = randInt(Partitions[i-1] + 2,Partitions[i] - width2);
            my2.left_column = col2;
            my2.right_column = col2 + width2 - 1;
        }
        
        //PUSHING NORMAL ROOM DATA
        my.left_column = col;
        my.right_column = col + width - 1;
        data.push_back(my);
        
        //Call the function called mark rooms which clears the space in the grid and number codes the grid with the number being related to the vector it is pushed in
        Mark_Room(row, col, length, width, i);
        
        if(make_overlaps == true)
        {
            Mark_Room(row2, col2, length2, width2, i + 10);
            data2.push_back(my2);
            make_overlaps = false;
        }
            
    }

     //DRAW PATHS BETWEEN LINEAR ROOMS
     /*
      By linear rooms here I refer to the rooms that were assigned to a section at the first point
      */
     for(int i = 0; i < num_rooms - 1; i++)
     {
         int start = randInt(data[i].top_row, data[i].bot_row);
         int end = randInt(data[i + 1].left_column, data[i + 1].right_column);
         bool touched = false;
         //IF IN LINE
         /*
          Here as all the rooms are number marked basically what I am doing is I am Making Paths until and reach my destined row/col...Now if on that row and column if I am also in my destined room then my work is done and I can exxit the loop or else then I wll decide to move up or down
          */
         for(int j = data[i].right_column ; j != end ; j++)
         {
             m_grid[start][j] = 9;

             if(m_grid[start][j + 1] == i + 1)
                 touched = true;
         }

         int direc;

         if(start > data[i+1].bot_row)
             direc = -1;
         else
         {
             direc = 1;
         }

         //IF NOT IN LINE(UP-DOWN)
         if(!touched)
         {
             for(int k = start; m_grid[k][end] != i + 1; k += direc)
             {
                 m_grid[k][end] = 9;
             }
         }
     }
     
    
     //DRAW PATHS BETWEEN OVERLAP ROOMS
     
     for(int i = 0; i < data2.size(); i++)
     {
         int start = randInt(data2[i].left_column, data2[i].right_column);
         
         bool touched = false;
         //IF IN LINE
         int p_d = data2[i].data1;
         
         //DECIDE ACC.TO ORIENATATION
         int exp_move = 1;
         
         if(start < data[p_d].left_column)
         {
             exp_move = 1;
         }

         if(start > data[p_d].right_column)
         {
             exp_move = -1;
         }
         /*
          Here as all the rooms are number marked basically what I am doing is I am Making Paths until and reach my destined row/col...Now if on that row and column if I am also in my destined room then my work is done and I can exxit the loop or else then I wll decide to move left or right
          */
        
         //IF OVERLAP IS UP
         if(data2[i].pos == UP)
         {
             for(int j = data2[i].bot_row ; m_grid[j][start] != p_d && m_grid[j][start] != 9 && j != data[p_d].top_row; j += UP)
             {
                 m_grid[j][start] = 9;

                 if(m_grid[j + 1][start] == p_d || m_grid[j + 1][start] == 9)
                     touched = true;
             }
             
             if(!touched)
             {
                 for(int k = start; m_grid[data[p_d].top_row][k] != p_d; k += exp_move)
                 {
                     m_grid[data[p_d].top_row][k] = 9;
                 }
                 touched = true;
             }
         }
         
         //IF OVERLAP IS DOWN
         if(data2[i].pos == DOWN)
         {
             for(int j = data2[i].top_row ; m_grid[j][start] != p_d && m_grid[j][start] != 9 && j != data[p_d].bot_row; j += DOWN)
             {
                 m_grid[j][start] = 9;

                 if(m_grid[j - 1][start] == p_d || m_grid[j - 1][start] == 9)
                     touched = true;
             }
             
             if(!touched)
             {
                 for(int k = start; m_grid[data[p_d].bot_row][k] != p_d; k += exp_move)
                 {
                     m_grid[data[p_d].bot_row][k] = 9;
                 }
                 touched = true;
             }
         }
     }
     
     
 
}

/*
 A function Just called by Create rooms
 Create roooms number marks each room that it creates
 This enables and aids the path making process
 */
void Dungeon::Mark_Room(int row,int col,int length,int width,int marker)
{
    for(int i = row ; i < row + length ; i++)
        for(int k = col ; k < col + width ; k++)
            m_grid[i][k] = marker;
}




/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//CHECKING SPOTS IN THE GRID
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//TRIVIAL GETTERS
//NAMES ARE SELF SUGGESTING
bool Dungeon::isPlayerAt(Position pos) const
{
    if(isPlayerplaced == true)
    {
        return pos.row == m_player->getPos().row && pos.col == m_player->getPos().col;
    }
    
    return false;
}

bool Dungeon::isStaircaseAt(Position pos) const
{
    if(m_level == 4)
    {
        return false;
    }
    return pos.row == m_stairs->getPos().row && pos.col == m_stairs->getPos().col;
}

bool Dungeon::isIdolAt(Position pos) const
{
    if(m_level != 4)
    {
        return false;
    }
    return pos.row == m_idol->getPos().row && pos.col == m_idol->getPos().col;
}

//which represents to the Index of Monster/object in the respective vector that is at the given point
bool Dungeon::isObjectAt(Position pos , int& which) const
{
    for(size_t k = 0;k < m_GameObject_vector.size(); k++)
    {
        if(m_GameObject_vector[k]->getPos().row == pos.row && m_GameObject_vector[k]->getPos().col == pos.col)
        {
            which = k;
            return true;
        }
    }
    
    return false;
}


//which represents to the Index of Monster/object in the respective vector that is at the given point
bool Dungeon::isMonsterAt(Position pos , int& which) const
{
    
    for(size_t k = 0;k < m_Monster_vector.size(); k++)
    {
        if(m_Monster_vector[k]->getPos().row == pos.row && m_Monster_vector[k]->getPos().col == pos.col)
        {
            which = k;
            return true;
        }
    }
    return false;
}


//checks if a spot is eligible to move onto or to be placed
bool Dungeon::isEligibleSpot(Position pos) const
{
    int dummy = 0;
    return !isWallAt(pos) && !isMonsterAt(pos, dummy) && !isPlayerAt(pos);
}



//checks if a spot is in the grid
bool Dungeon::isInGrid(Position pos) const
{
    return pos.row > 0 && pos.col > 0 && pos.row < MAX_D_ROWS - 1 && pos.col < MAX_D_COLUMNS - 1;
}


//Gets a spot eligible to move on and to be placed on
Position Dungeon::getEligiblePosition()
{
    int temp_row = 0;
    int temp_col = 0;
    Position temp_pos(temp_row,temp_col);
    
    //keep running the loop until an ELIGIBLE spot is found
    do
    {
        temp_row = randInt(1, MAX_D_ROWS - 2);
        temp_col = randInt(1, MAX_D_COLUMNS - 2);
        temp_pos.set(temp_row, temp_col);
    }while(!isEligibleSpot(temp_pos));
    
    return temp_pos;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//POPULATE DUNGEON
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//TRIVIAL
//SET PLAYERS DUNGEON
//SET PLAYER'S POSITION
void Dungeon::addPlayer()
{
    m_player->setDungeon(this);
    m_player->setPosition(getEligiblePosition());
    isPlayerplaced = true;
}




//ADD MONSTERS
void Dungeon::addMonsters(int goblinSmelldistance)
{
    int level = m_level;
    //number of monsters to be added
    int num_monsters = randInt(2,5*(level+1)+1);
    
    //which alll monsters will be there on the dungeon
    int monster_variety = 0; //Initialized to zero to silent warning
    
    //select which all monsters are appropriate for the level
    switch(level)
    {
        case 0 :
        case 1 :
            monster_variety = 1;
            break;
            
        case 2 :
            monster_variety = 2;
            break;
            
        case 3 :
        case 4 :
            monster_variety = 3;
            break;
    }
    
    
    char Monsters[4] = {SNAKEWOMEN , GOBLIN , BOGEYMEN , DRAGON};
    
    
    for(int i = 0 ; i < num_monsters ; i++)
    {
        //Randomly find the monster for this iteration
        char monster_selection = Monsters[randInt(0, monster_variety)];
        
        Position mon_pos;
        
        mon_pos = getEligiblePosition();
        
        
        switch(monster_selection)
        {
            case SNAKEWOMEN :

                m_Monster_vector.push_back(new Snakewoman(this,mon_pos));
                break;
                
            case GOBLIN :

                m_Monster_vector.push_back(new Goblin(this,mon_pos));
                //set goblin smell here
                m_Monster_vector.back()->setGoblinSmell(goblinSmelldistance);
                break;
                
            case BOGEYMEN :

                m_Monster_vector.push_back(new Bogeyman(this,mon_pos));
                break;
                
            case DRAGON :
                
                m_Monster_vector.push_back(new Dragon(this,mon_pos));
                break;
                
        }
    }
    
}





void Dungeon::addGameObjects()
{
    
    //Number of distributable Gameobjects is 2-3
    int num_objects = randInt(2, 3);
    
    //types of initially distributable GameObjects = 7
    
    Position t_pos;
    //if level is less than or equal to three add a staircase
    if(m_level <= 3)
    {
        t_pos = getEligiblePosition();
        m_stairs = new Staircase(this,t_pos);
    }
    
    // if the level is four add a Golden Idol
    if(m_level == 4)
    {
        t_pos = getEligiblePosition();
        m_idol = new GoldenIdol(this,t_pos);
    }
    
    for(int i = 0 ; i < num_objects ; i++)
    {
        int object_choice = randInt(1, 7);
        
        Position pos;
        
        int dummy;
        
        //using getspot as objects will always be distributed before actors
        //do-while loop to ensure that no two objects are placed at the same point !
        do
        {
            pos = getEligiblePosition();
        }while(isObjectAt(pos, dummy) || isStaircaseAt(pos) || isIdolAt(pos));
        
        switch(object_choice)
        {
            case 1 :
                //MACES
                m_GameObject_vector.push_back(new Mace(this,pos));
                break;
            case 2 :
                //SHORT SWORDS
                m_GameObject_vector.push_back(new ShortSword(this,pos));
                break;
            case 3 :
                //LONG SWORDS
                m_GameObject_vector.push_back(new LongSword(this,pos));
                break;
            case 4 :
                //ARMOR SCROLL
                m_GameObject_vector.push_back(new ArmorScroll(this,pos));
                break;
            case 5 :
                //STRENGTH_SCROLL
                m_GameObject_vector.push_back(new StrengthScroll(this,pos));
                break;
            case 6 :
                //HEALTH_SCROLL
                m_GameObject_vector.push_back(new HealthScroll(this,pos));
                break;
            case 7 :
                //DEXTERITY_SCROLL
                m_GameObject_vector.push_back(new DexterityScroll(this,pos));
                break;
                
                
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//MUTATORS
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

//MOVING MONSTERS
//POLYMORPHISM USED
string Dungeon::moveMonsters()
{
    string msg;
    for(size_t k = 0;k < m_Monster_vector.size(); k++)
    {
        msg += m_Monster_vector[k]->move();
    }
    return msg;
}




void Dungeon::killMonster()
{
    char symbol;
    Position pos;
    
    for(size_t k = 0;k < m_Monster_vector.size(); k++)
    {
//        If any monster returns true to the call of isDead function
//        1)    delete that monster
//        2)    erase the position from the vector
//        3)    over a switch case drop GameObjects with the required probability for the                   Monster which is dead
//

        if(m_Monster_vector[k]->isDead() == true)
        {
            //COLLECT ITS DATA AND KILL
            symbol = m_Monster_vector[k]->getSymbol();
            pos = m_Monster_vector[k]->getPos();
            
            delete(m_Monster_vector[k]);
            m_Monster_vector.erase(m_Monster_vector.begin() + k);
            
            bool drop_prob;
            int dummy;  //Will Correspond to which object
            int random;
            
            //DROP OBJECTS -> TRIVIAL
            //SWITCH CASES FOR SELECTION
            switch(symbol)
            {
                case SNAKEWOMEN :
                    drop_prob = trueWithProbability(1.0/3.0);
                    
                    if(drop_prob == true && !isObjectAt(pos, dummy))
                    {
                        m_GameObject_vector.push_back(new MagicFangs(this,pos));
                    }
                    break;
                    
                case GOBLIN :
                    drop_prob = trueWithProbability(1.0/3.0);
                    
                    if(drop_prob == true && !isObjectAt(pos, dummy))
                    {
                        random = randInt(0, 1);
                        if(random == 1)
                        {
                            m_GameObject_vector.push_back(new MagicAxe(this,pos));
                        }
                        
                        else
                        {
                            m_GameObject_vector.push_back(new MagicFangs(this,pos));
                        }
                    }
                    
                    break;
                    
                case BOGEYMEN :
                    drop_prob = trueWithProbability(1.0/10.0);
                    
                    if(drop_prob == true && !isObjectAt(pos, dummy))
                    {
                        m_GameObject_vector.push_back(new MagicAxe(this,pos));
                    }
                    break;
                    
                case DRAGON :
                    
                    random = randInt(1, 5);
                    if(!isObjectAt(pos, dummy))
                    {
                        switch(random)
                        {
                            case 1:
                                m_GameObject_vector.push_back(new StrengthScroll(this,pos));
                                break;
                            case 2:
                                m_GameObject_vector.push_back(new HealthScroll(this,pos));
                                break;
                            case 3:
                                m_GameObject_vector.push_back(new DexterityScroll(this,pos));
                                break;
                            case 4:
                                m_GameObject_vector.push_back(new TeleportScroll(this,pos));
                                break;
                            case 5:
                                m_GameObject_vector.push_back(new ArmorScroll(this,pos));
                                break;
                        }
                    }
                    break;
                    
            }
        }
    }
    
}


//Used when Player Picks up and Object
void Dungeon::deleteObjectfromGrid(int index)
{
    m_GameObject_vector.erase(m_GameObject_vector.begin() + index);
}


//////////////////////////////////////////////////////////////////////////////
/////////////////////DUNGEON DESTRUCTOR
//////////////////////////////////////////////////////////////////////////////

Dungeon::~Dungeon()
{
    
    //DELETE MONSTER VECTOR
    
    for(int i = 0; i < m_Monster_vector.size() ; i++)
    {
        delete m_Monster_vector[i];
//        m_Monster_vector.erase(m_Monster_vector.begin() + i);
    }
    
    //DELETE GAMEOBJECT VECTOR 
    
    for(int i = 0; i < m_GameObject_vector.size() ; i++)
    {
        delete m_GameObject_vector[i];
//        m_GameObject_vector.erase(m_GameObject_vector.begin() + i);
    }
    
    if(m_level <= 3)
    {
        delete m_stairs;
    }
    
    if(m_level == 4)
    {
        delete m_idol;
    }
    
}
