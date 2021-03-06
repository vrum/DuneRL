/*
 *  Player.cpp
 *  DuneRL
 *
 *  Created by Daniel on 12/03/12.
 *  Copyright 2012 nocturnalcode.com. All rights reserved.
 *
 */

#include "Player.h"
#include "Stringer.h"
#include "DuneRL.h"
#include "Tile.h"
#include "LightFilterDayNightCycle.h"
#include "LightFilterShadow.h"
#include "LightFilterSpiceMadness.h"
#include "Melee.h"
#include "Ranged.h"
#include "Shield.h"
#include "Spice.h"
#include "Blood.h"
#include "Corpse.h"
#include "Effect.h"
#include "DuneTile.h"
#include "SpiceMadness.h"

#include "RangeFilter.h"

#include "DuneWorld.h"

Player::Player() : Monster(new Ascii(64,Colour::red(), Colour::clear()))//Monster(new Ascii(64,1.0,0,0))
{
    if(arc4random()%2==0)
    {
        name = "Feyd Rautha";
        family = "Harkonnen";
        oid = 24;
    }
    else
    {
        name = "Leto Atreides";
        family = "Atreides";
        oid = 42;
    }
    
    darticle = "";
    iarticle = "";
	behaviour = BehaviourNone;
    sight = 45;
    setMaxHP(10);
    maxWater = (TURNS_IN_A_DAY * 1.5f/15.0f)/3;
    water = maxWater;
    waterTick = 0;
    rateOfDehydration = TURNS_IN_A_DAY_PART;
    kills = 0;
    description = "You";
    
    inventory = new Objects();
    equipment = new ObjectMap();
    
    Melee *melee = new Melee();
    addObjectToInventory(melee);
    equip(melee);
    
    Ranged *ranged = new Ranged();
    addObjectToInventory(ranged);
    equip(ranged);
}

Player::~Player()
{
    printf("Destructing Player");
}

int Player::spiceCount()
{
    int count = 0;
    foreachp(Objects, o, inventory)
    {
        Spice *spice = dynamic_cast<Spice *>((*o));
        if(spice != NULL)
        {
            count = spice->count;
            break;
        }
    }
    return count;
}

std::string Player::spiceDescription()
{
    return stringFormat("%d Spice",spiceCount());
}

SpiceMadness *Player::spiceMadnessEffect()
{
    foreach(Effects, e, effects)
    {
        SpiceMadness *madness = dynamic_cast<SpiceMadness *>((*e));
        if(madness)
        {
            return madness;
        }
    }
    
    return NULL;
}

std::string Player::waterDescription()
{
    std::string description;
    
    SpiceMadness *madness =  spiceMadnessEffect();
    
    if(madness && madness->isMad())
        description = "#0ffSpice Madness";
    else if(madness)
        description = "#0ffMild Spice Madness";
    else
    {    
        if(water <= 0)
            description = "#f00Dying"; // you'll be losing health now
        else if(water <= maxWater/2)
            description = "#f82Dehydrated";
        else if(water <= 3*maxWater/4)
            description = "#ff0Thirsty";
        else
            description = "#0f0Hydrated";
    }
    
    return description + stringFormat("< %d/%d>",water,maxWater);
}

std::string Player::underfootDescription()
{
    if(isAlive() || DEV)
        return parent->description();
    return "You will stand amongt the dunes no more.";
}

void Player::onDeath()
{
    if(DEV)
        LOG("%s would have died, but is apparently too good for that.",this->name.c_str());
    else
    {
        Monster::onDeath();
        DuneRL::shared->deathMenu();
    }
}

bool Player::equip(Object *object)
{
    if(equipment == NULL)
        equipment = new ObjectMap();
    
    if(object->parent != NULL)
        addObjectToInventory(object);
    
    Melee *melee = dynamic_cast<Melee *>(object);
    if(melee != NULL)
    {
        Object *prev = (*equipment)["Melee"];
        if(prev)
            unequip(prev);
        
        (*equipment)["Melee"] = object;
        didEquipObject(object);
        // was equipped
        return true;
    }
    Ranged *ranged = dynamic_cast<Ranged *>(object);
    if(ranged != NULL)
    {
        Object *prev = (*equipment)["Ranged"];
        if(prev)
            unequip(prev);
        
        (*equipment)["Ranged"] = object;
        didEquipObject(object);
        
        // was equipped
        return true;
    }
    Shield *shield = dynamic_cast<Shield *>(object);
    if(shield != NULL)
    {
        Object *prev = (*equipment)["Shield"];
        if(prev)
            unequip(prev);
        
        (*equipment)["Shield"] = object;
        didEquipObject(object);
        
        // was equipped
        return true;
    }
    return false;
}

bool Player::isAlive()
{
    if (DEV) {
        return true;
    }
    else {
        return Monster::isAlive();
    }
}

// a turn has past
void Player::performTurn()
{
    SpiceMadness *madness = spiceMadnessEffect();
    //decrement water;
    if(madness==NULL)
        waterTick++;
    
    if(waterTick >= rateOfDehydration)
    {
        LOG("You feel thirstier.");
        waterTick = 0;
        water--;
    }
    
    if(water < 0 && (madness==NULL))
    {
        if(!DEV)
        {
            LOG("Losing health from dehydration.");
            adjustHP(-1);
        }
    }
}

void Player::calculateSight()
{
    if(isAlive())
    {
        if (sightMap == NULL) {
            sightMap = new Lightmap(getPosition(),sight,getMap());
            sightMap->addFilter(new LightFilterShadow());
            sightMap->addFilter(new LightFilterDayNightCycle());
            
        }
        sightMap->setPosition(getPosition());
//        RangeFilter *range = new RangeFilter(Point(4,10));//
//        range->setMaxRange(15);
//        range->setDestinationPoint(Point(-4,10));
//        sightMap->addFilter(range);
        
    }
}

void Player::didEquipObject(Object *object)
{
    Monster::didEquipObject(object);
    LOG("Equipped %s",object->name.c_str());
}

void Player::didUnequipObject(Object *object)
{
    Monster::didUnequipObject(object);
    LOG("Unequipped %s",object->name.c_str());
}

void Player::didDropObject(Object *object)
{
    Monster::didDropObject(object);
    LOG("Dropped %s",object->name.c_str());
}

void Player::didPickupObject(Object *object)
{
    Monster::didPickupObject(object);
    LOG("Picked up %s",object->name.c_str());
}

void Player::didEnterTile(Tile *tile)
{
    Monster::didEnterTile(tile);
    
    DuneTile *t = dynamic_cast<DuneTile *>(tile);
    if(tile && t->getGroundType() == GroundTypeFortress)
    {
        // you won, yay!
        LOG("You've reach safety at last.");
        DuneRL::shared->winMenu();
    }
}

void Player::hydrate(int amount)
{
    water += amount;
    if(water > maxWater)
    {
        int health = water - maxWater;
        water = maxWater;

        if(health > 0)
            adjustHP(health);
    }
}

void Player::didConsumeObject(Object *object)
{
    Monster::didConsumeObject(object);
    LOG("Consumed %s",object->name.c_str());
    
    Spice *spice = dynamic_cast<Spice *>(object);
    if(spice)
    {
        // spice madness is a weak poison type of effect, if duration is over a threshold, bad effects happen
        SpiceMadness *spiceEffect = new SpiceMadness(50+(arc4random()%100),this);
        addEffect(spiceEffect);
        return;
    }
    
    Blood *blood = dynamic_cast<Blood *>(object);
    if(blood)
    {
        // get some hydration
        hydrate(1);
        return;
    }
    
    Corpse *corpse = dynamic_cast<Corpse *>(object);
    if(corpse)
    {
        // get some hydration
        hydrate(arc4random()%5);
        return;
    }
}

