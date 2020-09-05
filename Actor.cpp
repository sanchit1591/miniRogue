//
//  Actor.cpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/20/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#include "Actor.h"
#include "Dungeon.h"
#include "cstdlib"
#include <queue>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//ACTOR FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


string Actor::attack(Actor* attacker , Actor* defender)
{
    //Setting up the attack string
    string attackString = attacker->m_name + attacker->getWeapon()->getVerb() + defender->m_name;
    
    //Calculating stats for attack by given formulaes
    int attackerPoints = attacker->getDexterityPoints() + attacker->getWeapon()->getDexterity();
    int defenderPoints = defender->getDexterityPoints() + defender->getArmorPoints();
    int damagePoints = randInt(0, attacker->getStrengthPoints() + attacker->getWeapon()->getDamageAmt() - 1);
    
    bool slp_time_prob;
    
    if(getdungeon()->getPlayer()->isDead() == true)
    {
        return "";
    }
    
    //If the conditions are such that attacker will hit
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
    {
        defender->updateHitPoints(SUBTRACT, damagePoints);
        
        //if defender dies
        if(defender->getHitpoints() <= 0)
        {
             attackString += "dealing a final blow.";
            
            //KILLL PLAYER/MONSTER
            
            defender->setDead();
            //if not a player than kill Monster
            if(defender->getSymbol() != PLAYER)
            {
                getdungeon()->killMonster();
            }
            
        }
        
        //if weapon is magic fangs
        else if(attacker->getWeapon()->getObjName() == "magic fangs of sleep")
        {
            attackString += "and hits";
            slp_time_prob = trueWithProbability(1.0/5.0);
            
            //if sleep probablity is true update sleep time
            if(slp_time_prob == true)
            {
                //If not in sleep already
                if(getSleepTime() == 0)
                {
                    defender->updateSleepTime(ADD, randInt(2, 6));
                }
                
                //If already in sleep
                else
                {
                    int temp = getSleepTime();
                    //Make sleep time 0
                    defender->updateSleepTime(SUBTRACT, getSleepTime());
                    //Updating sleep time to be the MAX of two
                    defender->updateSleepTime(ADD, max(randInt(2, 6), temp));
                }
                attackString += ", putting " + defender->m_name + "to sleep";
            }
            attackString += ".";
        }
        
        else
        {
            attackString += "and hits.";
        }
    }
    
    else
    {
        attackString += "and misses.";
    }
    
    return attackString + "\n";
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//PLAYER MOVES
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


string Player::move(char direction)
{
    Position current = getPos();
    string move_string = "";
    int which_mon;
    Monster* monster;
    
    //update local variables according to move
    switch(direction)
    {
        case ARROW_UP: current.set(getPos().row - 1, getPos().col); break;
        case ARROW_DOWN: current.set(getPos().row + 1, getPos().col); break;
        case ARROW_LEFT: current.set(getPos().row , getPos().col - 1); break;
        case ARROW_RIGHT: current.set(getPos().row , getPos().col + 1); break;
    }
    
    //If valid path then move
    if(getdungeon()->isEligibleSpot(current))
        setPosition(current);
    
    //If a monster is present then attack
    else if (getdungeon()->isMonsterAt(current, which_mon))
    {
        monster = getdungeon()->getMonster(which_mon);
        move_string += attack(this, monster);
    }
    
    //if wall do nothing
    return move_string;
        
}


string Player::pickObject()
{
    string msg = "";
    
    //If Idol is Picked
    if(getdungeon()->isIdolAt(getPos()))
    {
        m_won = true;
        msg += "You pick up the golden idol\n";
        msg += "Congratulations, you won!\n";
        return msg;
    }
    
    int which_obj;  //index of object being picked .......
    GameObject* objectPicked;
    if(getdungeon()->isObjectAt(getPos(),which_obj) == true && isInventoryfull() == false)
    {
        objectPicked = getdungeon()->getObject(which_obj);
        inventory.push_back(objectPicked);  //put in inventory
        
        //Efficient String Concatenation
        msg += "You pick up ";
        if(objectPicked->getSymbol() == SCROLLS)
        {
            msg += "a scroll called ";
        }
        
        msg += objectPicked->getObjName();
        
        //erase from grid
        getdungeon()->deleteObjectfromGrid(which_obj);
    }
    
    else if(isInventoryfull() == true)
    {
        msg += "Your knapsack is full; you can't pick that up.\n";
    }
    return msg + "\n";
}

//DISPLAY'S INVENTORY
void Player::displayInventory()
{
    char bullets;
    
    clearScreen();
    cout<<"Inventory:"<<endl;
    for(size_t k = 0; k < inventory.size() ; k++)
    {
        bullets = k + 97;   //Using ASCII
        cout<<" "<<bullets<<". "<<inventory[k]->getObjName()<<endl;
    }
}

string Player::weildWeapon()
{
    string msg = "";
    char which = getCharacter();
    
    //take index and convert to number
    GameObject* W_weapon = inventory[which - 97];
    Weapon* weild = dynamic_cast<Weapon*>(W_weapon);    //DYNAMIC CASTING TO GET WEAPON POINTER FROM BASE CLASS POINTER
    
    //if demanded index is greater than inventory size
    if(which - 97 >= inventory.size())
    {
        return msg;
    }
    
    //if casted pointer points to a weapon
    else if(weild != nullptr)
    {
        msg += "You are wielding ";
        setWeapon(weild);
    }
    
    //if casted pointer dosen't point to a weapon
    else
    {
        msg += "You can't wield ";
    }
    
    msg += W_weapon->getObjName() + "\n";
    
    return msg;
}


string Player::readScroll()
{
    char which = getCharacter();
    GameObject* R_Scroll = inventory[which - 97];
    Scroll* read = dynamic_cast<Scroll*>(R_Scroll);  //DYNAMIC CASTING TO GET WEAPON POINTER FROM BASE CLASS POINTER
    
    string msg = "";
    
    //if demanded index is greater than inventory size return empty string
    if(which - 97 >= inventory.size())
    {
        return msg;
    }
    
    //if casted pointer points to a scroll read it
    else if(read != nullptr)
    {
        msg += "You read the scroll called " + read->getObjName() + "\n";
        msg += read->updatePlyrStats() + "\n";

        //"MAGICALLY DESTRUCTED" Scroll
        Scroll* tobedeleted = (Scroll *)inventory[which - 97];
        inventory.erase(inventory.begin() + which - 97);
        delete tobedeleted;
    }
    
    //if casted pointer dosen't point to a scroll don't read
    else
    {
        msg += "You can't read a " + R_Scroll->getObjName() + "\n";
    }
    
    return msg;
    
}

//PLAYER CHEATING
//Set hit points to 50
//Set strength points to 9
void Player::cheat()
{
    m_maxhitPoints = CHEAT_HIT_POINTS;

    updateHitPoints(SUBTRACT, getHitpoints());
    updateHitPoints(ADD, CHEAT_HIT_POINTS);
    
    updateStrengthPoints(SUBTRACT, getStrengthPoints());
    updateStrengthPoints(ADD, CHEAT_STRENGTH_POINTS);
    
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//MONSTER MOVES
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOGEYMAN AND SNAKEWOMEN SENSING FUNCTION
bool Monster::isPlayerinRange(int steps , Position mon_pos )
{
    Position monster_pos = mon_pos;
    int m_row = monster_pos.row;
    int m_col = monster_pos.col;
 //m_row + steps --> range below
 //m_row - steps --> range above
    
 //These two loops are traversing the entire rhombus area of influence
 
    for(int i = m_row ; i <= m_row + steps ; i++ )
    {
        for(int j = m_col - (m_row + steps - i); j <= m_col + (m_row + steps - i );j++)
        {
            monster_pos.set(i, j);
            if(getdungeon()->isInGrid(monster_pos) && getdungeon()->isPlayerAt(monster_pos))
                return true;
        }
    }
    
 //BELOW SHOWN IS THE AREA OF INFUENCE OF A SNAKEWOMEN
 //          *
 //        * * *
 //      * * * * *
 //    * * * * * * *
 //      * * * * *
 //        * * *
 //          *

    
 //ending at m_row - 1 because middle row traversed above
    
    for(int i = m_row - steps ; i <= m_row - 1 ; i++ )
    {
        for(int j = m_col - (i - (m_row - steps)); j <= m_col + (i - (m_row - steps)); j++)
        {
            monster_pos.set(i, j);
            if(getdungeon()->isInGrid(monster_pos) && getdungeon()->isPlayerAt(monster_pos))
                return true;
        }
    }
 
 return false;
}

//USES IDEAS FROM MATHS TO FIND DISTANCE BETWEEN TWO POINTS
int Monster::distancefromPlayer(Position p_pos,Position m_pos)
{
    int m_row = m_pos.row;
    int p_row = p_pos.row;
    int m_col = m_pos.col;
    int p_col = p_pos.col;
    return (m_row - p_row)*(m_row - p_row) + (m_col - p_col)*(m_col - p_col);
}

//Proposes a move ....All proposals are surveyed by the move function and then a move is made
//returns distance of player from the proposed move spot
bool Monster::attemptmove(char direction , int& distance)
{
    Position current = getPos();
    
    //update local variables according to move
    switch(direction)
    {
        case ARROW_UP: current.set(current.row - 1, current.col); break;
        case ARROW_DOWN: current.set(current.row + 1, current.col); break;
        case ARROW_LEFT: current.set(current.row , current.col - 1); break;
        case ARROW_RIGHT: current.set(current.row , current.col + 1); break;
    }
    
    int dummy;
    distance = distancefromPlayer(getdungeon()->getPlayer()->getPos(),current);
    
    if(getdungeon()->isWallAt(current))
        return false;
    
    else if(getdungeon()->isMonsterAt(current, dummy) || getdungeon()->isPlayerAt(current))
        return false;   //DID IS ACTOR AT AND RETURNED FALSE AS PRESENCE OF PLAYER CAN BE TESTED INSIDE WITH THE IS PLAYER PRESENT FUNC.
    
    
    else
        return true;
    
    
}


string Monster::move()
{
    string msg = "";
    //If SleepTime is not 0 , reduce it by 1 and return empty string
    if(this->getSleepTime() != 0)
    {
        updateSleepTime(SUBTRACT, 1);
        return msg;
    }
    vector<Position> movechoices;
    
    
    int currentdistance = distancefromPlayer(getdungeon()->getPlayer()->getPos(), getPos());
    int attemptdistance;
    
    //if player in adjacent blocks
    if(isPlayerinRange(1,getPos()))
    {
        msg += attack(this, getdungeon()->getPlayer());
        return msg;
    }
    
    //Checks for player in range and the distance change on potential movement
    //attemptdistance <= currentdistance --> Checks that is the movement in proposed direction taking the monster away from the player
    else if(isPlayerinRange(m_smell,getPos()))
    {
        //try moving up
        if(attemptmove(ARROW_UP, attemptdistance) && attemptdistance <= currentdistance)
            movechoices.push_back(Position(getPos().row - 1, getPos().col));
        
        //try moving down
        if(attemptmove(ARROW_DOWN, attemptdistance) && attemptdistance <= currentdistance)
            movechoices.push_back(Position(getPos().row + 1, getPos().col));
        
        //try moving left
        if(attemptmove(ARROW_LEFT, attemptdistance) && attemptdistance <= currentdistance)
            movechoices.push_back(Position(getPos().row, getPos().col - 1));
        
//        try moving right
        if(attemptmove(ARROW_RIGHT, attemptdistance) && attemptdistance <= currentdistance)
            movechoices.push_back(Position(getPos().row, getPos().col + 1));
        
        //IF NO MOVE
        if(movechoices.size() == 0)
            return "";
        
        //If multiple options/possible paths are present
        //Randomly choose
        int way_to_go = randInt(0,movechoices.size() - 1);
        
        setPos(movechoices[way_to_go]);
        
        return msg;
    }
    
    else
    {
        return msg;
    }
    

}


string Dragon::move()
{
    bool hitPointsUpdate_prob = trueWithProbability(0.1);
    
    //update hit points 
    if(hitPointsUpdate_prob == true && this->getHitpoints() < 25)
    {
        this->updateHitPoints(ADD, 1);  //UPDATING HIT POINTS
    }
    
    //CHECKING IF PLAYER IS IN RANGE OF ADJACENT BLOCKS
    //  *
    //*   *
    //  *
    if(isPlayerinRange(1,getPos()))
    {
        return attack(this,getdungeon()->getPlayer());
    }
    return "";
}

//Can the goblin move on the proposed spot?
bool Goblin::movableSpot(int row, int col)
{
    Dungeon *d = getdungeon();
    int dummy;
    //Check for wall , monster and precence of wall
    return (!d->isWallAt(Position(row,col)) && !d->isMonsterAt(Position(row,col),dummy) && !d->isVisitedbyGoblin(Position(row,col)));
}

int Goblin::pathExists(Position player,int max_smell)
{
    //1.    if the queue is empty return -1
    if(data.empty())
    {
        return -1;
    }
    
    //    pop the top co-ordinate of the queue and save that in a local variable
    spotData front = data.front();
    data.pop();
    
    //  2.    if the distance part of any coordinate pushed into the queue exceeds max_smell and player is not found return -1
    if(front.distance == max_smell && (player.row != front.row || player.col != front.col))
    {
        return -1;
    }
    
    // 3.    if player is found then return the distance of the player from the starting co-ordinate
    if(player.row == front.row && player.col == front.col)
    {
        return front.distance;
    }
    
    
    
    // 4.    For each of the Four directions from the current spot , if the spot is a valid spot then
    
    //    mark the spot visited
    //    push it on the queue with the position and the distance increased by 1 as compared to the. popped item's distance


    if (movableSpot(front.row - 1, front.col))
    {
        getdungeon()->markVisitedbyGoblin(Position(front.row - 1, front.col));
        data.push({front.row - 1,front.col,front.distance + 1});
    }
    
    if (movableSpot(front.row + 1, front.col))
    {
        getdungeon()->markVisitedbyGoblin(Position(front.row + 1, front.col));
        data.push({front.row + 1, front.col,front.distance + 1});
    }
    
    if (movableSpot(front.row, front.col + 1))
    {
        getdungeon()->markVisitedbyGoblin(Position(front.row, front.col + 1));
        data.push({front.row, front.col + 1,front.distance + 1});
    }
    
    if (movableSpot(front.row, front.col - 1))
    {
        getdungeon()->markVisitedbyGoblin(Position(front.row, front.col - 1));
        data.push({front.row, front.col - 1,front.distance + 1});
    }
     
    //RECURRSIVE CALL
    
    return pathExists(player,max_smell);

}

//CLEAR THE QUEUE
void Goblin::clearData()
{
    while(data.size() != 0)
    {
        data.pop();
    }
}

string Goblin::move()
{
    //1.    Set up the move string
    string msg = "";
    
    //2.    if sleeptime is not 0 , reduce it by one and return empty string
    if(this->getSleepTime() != 0)
    {
        updateSleepTime(SUBTRACT, 1);
        return msg;
    }
    
    //if player in adjacent blocks -> attack
    if(isPlayerinRange(1, getPos()))
    {
        msg += attack(this, getdungeon()->getPlayer());
        return msg;
    }
    
    int dummy;
    Position p_pos = getdungeon()->getPlayer()->getPos();
    
    //4.    Set up a steps array which will store the number of steps for each of the direction goblin moves in and initialize them all to -1
    int stepsArr[4] = {-1,-1,-1,-1};
    
    //5.    For each of the four directions
    //    Mark the first step goblin would take in that direction as visited
    //    push that in the queue which will be used in recurrsion
    //    call the recurrsive function path exists and store the value it returns in the array
    //    Unmark all the visited spots
    //    clear up the data queue

    
    if(attemptmove(ARROW_UP, dummy))
    {
        getdungeon()->markVisitedbyGoblin(Position(getPos().row - 1, getPos().col));
        data.push({getPos().row - 1,getPos().col,0});
        stepsArr[0] = pathExists(p_pos,getGoblinSmell() - 1);
        getdungeon()->unMarkVisited();
        clearData();
    }
    
    if(attemptmove(ARROW_DOWN, dummy))
    {
        getdungeon()->markVisitedbyGoblin(Position(getPos().row + 1, getPos().col));
        data.push({getPos().row + 1,getPos().col,0});
        stepsArr[1] = pathExists(p_pos,getGoblinSmell() - 1);
        getdungeon()->unMarkVisited();
        clearData();
    }
    
    if(attemptmove(ARROW_LEFT, dummy))
    {
        getdungeon()->markVisitedbyGoblin(Position(getPos().row, getPos().col - 1));
        data.push({getPos().row,getPos().col - 1,0});
        stepsArr[2] = pathExists(p_pos,getGoblinSmell() - 1);
        getdungeon()->unMarkVisited();
        clearData();
    }
    
    if(attemptmove(ARROW_RIGHT, dummy))
    {
        getdungeon()->markVisitedbyGoblin(Position(getPos().row, getPos().col + 1));
        data.push({getPos().row,getPos().col + 1,0});
        stepsArr[3] = pathExists(p_pos,getGoblinSmell() - 1);
        getdungeon()->unMarkVisited();
        clearData();
    }
    
    int distance = 16;
    int index = -1;
    
    //6.    Select the direction with the minimum distance to the player
    
    for(int i = 0;i < 4;i++)
    {
        if(stepsArr[i] != -1 && stepsArr[i] < distance)
        {
            distance = stepsArr[i];
            index = i;
        }
    }
    
    //7.    Move the goblin in the selected direction
    if(index != -1)
    {
        switch(index)
        {
            case 0:
                setPos(Position(getPos().row - 1, getPos().col));
                break;
            case 1:
                setPos(Position(getPos().row + 1, getPos().col));
                break;
            case 2:
                setPos(Position(getPos().row, getPos().col - 1));
                break;
            case 3:
                setPos(Position(getPos().row, getPos().col + 1));
                break;
        }
    }
    
    return msg;
}






//int Goblin::pQuadrant(Position player,Position goblin)
//{
//    if(player.row < goblin.row && player.col > goblin.col)
//        return QUADRANT_ONE;
//    if(player.row < goblin.row && player.col < goblin.col)
//        return QUADRANT_TWO;
//    if(player.row > goblin.row && player.col < goblin.col)
//        return QUADRANT_THREE;
//    if(player.row < goblin.row && player.col > goblin.col)
//        return QUADRANT_FOUR;
//    if(player.row == goblin.row)
//    {
//        if(player.col > goblin.col)
//            return POSITIVE_X;
//        if(player.col < goblin.col)
//            return NEGATIVE_X;
//    }
//    if(player.col == goblin.col)
//    {
//        if(player.row > goblin.row)
//            return NEGATIVE_Y;
//        if(player.row < goblin.row)
//            return POSITIVE_Y;
//    }
//    return ORIGIN;
//}

//
//
//bool Goblin::directionArr(int arr [] , Position player , Position goblin , int smell)
//{
//
//    bool r1 , r2;
//    for(int i = 0; i < 4;i ++ )
//    {
//        switch(arr[i])
//            {
//                case N:
//                    if (movableSpot(goblin.row - 1, goblin.col)  &&  pathExists(player,Position(goblin.row - 1,goblin.col),smell- 1))
//                        return true;
//                    break;
//
//                case W:
//                    if (movableSpot(goblin.row, goblin.col - 1)  &&  pathExists(player,Position(goblin.row , goblin.col - 1),smell- 1))
//                        return true;
//                    break;
//                case S:
//                    if (movableSpot(goblin.row + 1, goblin.col)  &&  pathExists(player,Position(goblin.row + 1,goblin.col),smell- 1))
//                        return true;
//                    break;
//                case E:
//                    if (movableSpot(goblin.row, goblin.col + 1)  &&  pathExists(player,Position(goblin.row , goblin.col + 1),smell- 1))
//                        return true;
//                    break;
//                case NE:
//                    r1 = (movableSpot(goblin.row - 1, goblin.col)  &&  pathExists(player,Position(goblin.row - 1,goblin.col),smell- 1));
//                    r2 = (movableSpot(goblin.row, goblin.col + 1)  &&  pathExists(player,Position(goblin.row , goblin.col + 1),smell- 1));
//                    if(r1 && r2)
//                        return (steps[0] >= steps[1]) ? r1 : r2;
//                    else
//                        return (r1 || r2);
//                        break;
//
//                case NW:
//                    r1 = (movableSpot(goblin.row - 1, goblin.col)  &&  pathExists(player,Position(goblin.row - 1,goblin.col),smell- 1));
//                    r2 = (movableSpot(goblin.row, goblin.col - 1)  &&  pathExists(player,Position(goblin.row , goblin.col - 1),smell- 1));
//                    if(r1 && r2)
//                        return (steps[0] >= steps[1]) ? r1 : r2;
//                    else
//                        return (r1 || r2);
//                        break;
//                case SE:
//                    r1 = (movableSpot(goblin.row + 1, goblin.col)  &&  pathExists(player,Position(goblin.row + 1,goblin.col),smell- 1));
//                    r2 = (movableSpot(goblin.row, goblin.col + 1)  &&  pathExists(player,Position(goblin.row , goblin.col + 1),smell- 1));
//                    if(r1 && r2)
//                        return (steps[0] >= steps[1]) ? r1 : r2;
//                    else
//                        return (r1 || r2);
//                        break;
//                case SW:
//                    r1 = (movableSpot(goblin.row + 1, goblin.col)  &&  pathExists(player,Position(goblin.row + 1,goblin.col),smell- 1));
//                    r2 = (movableSpot(goblin.row, goblin.col - 1)  &&  pathExists(player,Position(goblin.row , goblin.col - 1),smell- 1));
//                    if(r1 && r2)
//                        return (steps[0] >= steps[1]) ? r1 : r2;
//                    else
//                        return (r1 || r2);
//                        break;
//                default:
//                    break;
//            }
//    }
//    return false;
//}

//bool Goblin::pathExists(Position player,Position goblin,int smell)
//{
//    if(pQuadrant(player, goblin) == ORIGIN)
//    {
//        steps.push_back(smell);
//        return true;
//    }
//
//    int x_disp = abs(player.col - goblin.col);
//    int y_disp = abs(player.row - goblin.row);
//
//    if(x_disp + y_disp > smell)
//    {
//        return false;
//    }
//
//    if(smell == 0)
//    {
//        return false;
//    }
//
//    getdungeon()->markVisitedbyGoblin(goblin);
//
//    if(pQuadrant(player, goblin) == QUADRANT_ONE)
//    {
//        int dir1[4] = {N,E,SW,O};
//        int dir2[4] = {E,N,SW,O};
//        if(y_disp > x_disp)
//        {
//            return directionArr(dir1, player, goblin, smell);
//        }
//        else
//        {
//            return directionArr(dir2, player, goblin, smell);
//        }
//    }
//
//    if(pQuadrant(player, goblin) == QUADRANT_TWO)
//    {
//        int dir1[4] = {N,W,SE,O};
//        int dir2[4] = {W,N,SE,O};
//        if(y_disp > x_disp)
//        {
//            return directionArr(dir1, player, goblin, smell);
//        }
//        else
//        {
//            return directionArr(dir2, player, goblin, smell);
//        }
//    }
//
//    if(pQuadrant(player, goblin) == QUADRANT_THREE)
//    {
//        int dir1[4] = {S,W,NE,O};
//        int dir2[4] = {W,S,NE,O};
//        if(y_disp > x_disp)
//        {
//            return directionArr(dir1, player, goblin, smell);
//        }
//        else
//        {
//            return directionArr(dir2, player, goblin, smell);
//        }
//    }
//
//    if(pQuadrant(player, goblin) == QUADRANT_FOUR)
//    {
//        int dir1[4] = {S,E,NW,O};
//        int dir2[4] = {E,S,NW,O};
//        if(y_disp > x_disp)
//        {
//            return directionArr(dir1, player, goblin, smell);
//        }
//        else
//        {
//            return directionArr(dir2, player, goblin, smell);
//        }
//    }
//
//}
