//
//  GameObject.cpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/20/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#include "GameObject.h"
#include "Dungeon.h"


/*
 ASK WHY THEY CAN'T BE INLINE
 */


string TeleportScroll::updatePlyrStats()
{
    Position pos;
    
    //getting eligible new position
    pos = getDungeon()->getEligiblePosition();
    
    //Moving Player Accordingly
    getDungeon()->getPlayer()->setPosition(pos);
    return getVerb();
}

//UPDATING PLAYER STATISTICS …………………………………
//getVerb --> returns the action string of scroll --> eg Your armor glows blue
string StrengthScroll::updatePlyrStats()
{
    getDungeon()->getPlayer()->updateStrengthPoints(ADD, randInt(1, 3));
    return getVerb();
}

string ArmorScroll::updatePlyrStats()
{
    getDungeon()->getPlayer()->updateArmorPoints(ADD, randInt(1, 3));
    return getVerb();
}

string HealthScroll::updatePlyrStats()
{
    getDungeon()->getPlayer()->increaseMaxHitPoints(randInt(3, 8));
    return getVerb();
}

string DexterityScroll::updatePlyrStats()
{
    getDungeon()->getPlayer()->updateDexterityPoints(ADD, 1);
    return getVerb();
}
