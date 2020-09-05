//
//  GameObject.hpp
//  PROJECT 3 FINAL
//
//  Created by Sanchit Agarwal on 5/20/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef GameObject_h
#define GameObject_h

#include "globals.h"


//as dungeon pointer is present in the Player
class Dungeon;

////////////////////////////////////////////////////////////////////////////
//GAMEOBJECT CLASS   <<<SUPER CLASS>
///////////////////////////////////////////////////////////////////////////

//Base Class
//Contains attributes that are common to both Weapon and Scroll
//Derived Classes -> Weapon , Scroll

//All GameObjects have a name and so name is stored in Base Class

class GameObject
{
    public:
    
    //I have two Constructors in GameObject as when a player/Monster has a weapon then determining poisition is
    
    //This Game Object Constructor will be called INDIRECTLY by the default constructor of the most derived classes ...This is to accomodate the sitaution when a player/Monster holds a weapon
        GameObject(string name): m_name(name) {};
    
        //4 main attributes of all objects
        GameObject(Dungeon* dungeon ,Position position, int symbol , string object_name)
        {
            m_pos = position;
            m_dungeon = dungeon;
            m_symbol = symbol;
            m_name = object_name;
        }
    
    /////////////////////////////////////////////////////////
    //    ACCESSORS
    /////////////////////////////////////////////////////////
        string getObjName() const {    return m_name;  }
        Position getPos() const {   return m_pos;    }
        char getSymbol() {  return m_symbol;    }
        Dungeon* getDungeon() {     return m_dungeon;     }
    
    
    //TO MAKE IT POLYMORPHIC ---> CHECK!!!!!
    
//    ~~~~~~DESTRUCTOR~~~~~~~
    virtual ~GameObject() {};
    
    private:
        Dungeon* m_dungeon;
    
    
        Position m_pos;
        char m_symbol;
        string m_name;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SUB-BASE CLASSES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//All Classes that are DIRECTLY DERIVED from GameObject but further some other classes are derived from them


///////////////////////////////////////////////////////////////
//WEAPON CLASS   <<SUB-BASE CLASS>
///////////////////////////////////////////////////////////////


//Derived Class from GamoObject
//Defines the commonalities of all Weapons

class Weapon : public GameObject
{
    public:
    //Constructor used when Weapon Placed on Dungeon
    Weapon(Dungeon* dungeon , Position position, int p_array [] , string name , string verb) : GameObject(dungeon , position, ')', name) , m_verb(verb) , m_pointArray(p_array) {};
    
    //Constructor called when weapon in Hand
    Weapon(int p_array [], string verb , string name) : GameObject(name) ,m_pointArray(p_array) , m_verb(verb) {};
    
    //ACCESSORS
    int getDexterity() const {     return m_pointArray[0];      }
    int getDamageAmt() {    return m_pointArray[1];    }
    string getVerb() {      return m_verb;      }
    
    virtual ~Weapon()
    {};
    

    private:
        string m_verb;
        int *m_pointArray;
    /*
     0 --> Dexterity Bonus
     1 --> Damage amount
     */
    
};


///////////////////////////////////////////////////////////////
//MONSTER CLASS
///////////////////////////////////////////////////////////////

//Sub Base class for different types of monsters
//Derived Class from Actor
//Defines the commonalities of all the Monsters

class Scroll : public GameObject
{
    public:
    Scroll(Dungeon* dungeon , Position position , string name , string verb) : GameObject(dungeon , position , '?' , name) {m_verb = verb;}

        //ACCESSORS
        string getVerb() {return m_verb;}
        
        //VITAL FUNCTION OF ALL SCROLLS ....
        //Function Declared to use Polymorphism Later
  //-------------------------------      //TODO:ASK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        virtual string updatePlyrStats() {return "";}  //make this only string type :)))
        
//        virtual int dummy() {return 0;}
        
        virtual ~Scroll() {};
        
    private:
        string m_verb;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CLASSES DERIVED FROM WEAPON
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////
//MACE CLASS
///////////////////////////////////////////////////////////////


class Mace : public Weapon
{
    public:
        //Default constructor used when weapons placed in Player's hand
        Mace() : Weapon(pointsArray,"swings mace at ","mace") {};
    
        //Constructor used when objects placed on board
        Mace(Dungeon* dungeon , Position position) : Weapon(dungeon , position , pointsArray, "mace" ,"swings mace at ") {};
    
    private:
    
        int pointsArray[2] = {0,2};
};


///////////////////////////////////////////////////////////////
//SHORT SWORD CLASS
///////////////////////////////////////////////////////////////


class ShortSword : public Weapon
{
    public:
        //Default constructor used when weapons placed in Player's hand
        ShortSword() : Weapon(pointsArray,"slashes short sword at ","short sword") {};
    
        //Constructor used when objects placed on board
        ShortSword(Dungeon* dungeon , Position position) : Weapon(dungeon , position , pointsArray, "short sword" ,"slashes short sword at ") {};
    
    private:
    
        int pointsArray[2] = {0,2};
};


///////////////////////////////////////////////////////////////
//LONG SWORD CLASS
///////////////////////////////////////////////////////////////

class LongSword : public Weapon
{
    public:
        //Default constructor used when weapons placed in Player's hand
        LongSword() : Weapon(pointsArray,"slashes long sword at ","long sword") {};
    
        //Constructor used when objects placed on board
        LongSword(Dungeon* dungeon , Position position) : Weapon(dungeon , position , pointsArray, "long sword" ,"slashes long sword at ") {};
    
    private:
    
        int pointsArray[2] = {2,4};
};



///////////////////////////////////////////////////////////////
//MAGIC AXE CLASS
///////////////////////////////////////////////////////////////

class MagicAxe : public Weapon
{
    public:
        //Default constructor used when weapons placed in Player's hand
        MagicAxe() : Weapon(pointsArray,"chops magic axe at ","magic axe") {};
    
        //Constructor used when objects placed on board
        MagicAxe(Dungeon* dungeon , Position position) : Weapon(dungeon , position , pointsArray, "magic axe" ,"chops magic axe at ") {};
    
    private:
    
        int pointsArray[2] = {5,5};
};


///////////////////////////////////////////////////////////////
//MAGIC FANGS CLASS
///////////////////////////////////////////////////////////////

//Class Name plural because attacker shoots them as Plural
//eg - > "the Snakewoman strikes magic ****fangs****"

class MagicFangs : public Weapon
{
    public:
        //Default constructor used when weapons placed in Player's hand
        MagicFangs() : Weapon(pointsArray,"strikes magic fangs at ","magic fangs of sleep") {};
    
        //Constructor used when objects placed on board
        MagicFangs(Dungeon* dungeon , Position position) : Weapon(dungeon , position , pointsArray, "magic fangs of sleep" ,"strikes magic fangs at ") {};
    
    private:
    
        int pointsArray[2] = {3,2};
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CLASSES DERIVED FROM SCROLL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////
//TELEPORT SCROLL CLASS
///////////////////////////////////////////////////////////////


class TeleportScroll : public Scroll
{
    public:
        TeleportScroll(Dungeon* dungeon , Position position) : Scroll(dungeon , position, "scroll of teleportation" , "You feel your body wrenched in space and time.") {};
    virtual string updatePlyrStats();
    
};


///////////////////////////////////////////////////////////////
//ARMOR SCROLL CLASS
///////////////////////////////////////////////////////////////


class ArmorScroll : public Scroll
{
    public:
        ArmorScroll(Dungeon* dungeon , Position position) : Scroll(dungeon , position, "scroll of enhance armor" , "Your armor glows blue.") {};
        virtual string updatePlyrStats();
};


///////////////////////////////////////////////////////////////
//STRENGTH SCROLL CLASS
///////////////////////////////////////////////////////////////

class StrengthScroll : public Scroll
{
    public:
        StrengthScroll(Dungeon* dungeon , Position position) : Scroll(dungeon , position, "scroll of strength" , "Your muscles bulge.") {};
        virtual string updatePlyrStats();
};



///////////////////////////////////////////////////////////////
//HEALTH SCROLL CLASS
///////////////////////////////////////////////////////////////

class HealthScroll : public Scroll
{
    public:
        HealthScroll(Dungeon* dungeon , Position position) : Scroll(dungeon , position, "scroll of enhance health" , "You feel your heart beating stronger.") {};
        virtual string updatePlyrStats();
};


///////////////////////////////////////////////////////////////
//DEXTERITY SCROLL CLASS
///////////////////////////////////////////////////////////////



class DexterityScroll : public Scroll
{
    public:
        DexterityScroll(Dungeon* dungeon , Position position) : Scroll(dungeon , position, "scroll of enhance dexterity" , "You feel like less of a klutz.") {};
        virtual string updatePlyrStats();
};

////////////////////////////////////////////////////////////////
//OTHER GAME OBJECTS
////////////////////////////////////////////////////////////////

class Staircase : public GameObject
{
public:
    Staircase(Dungeon* dungeon , Position position) : GameObject(dungeon,position,'>',"Staircase") {};
};

class GoldenIdol : public GameObject
{
public:
    GoldenIdol(Dungeon* dungeon , Position position) : GameObject(dungeon,position,'&',"Golden Idol") {};
};
#endif /* GameObject_h */
