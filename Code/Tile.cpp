/*
 *  Tile.cpp
 *  Roguelike
 *
 *  Created by Daniel on 6/02/10.
 *  Copyright 2010 quantoaduro.com. All rights reserved.
 *
 */

#include "Map.h"
#include "Tile.h"
#include "Point.h"

#include <list>
#include <iostream>

Tile::Tile()
{
	Tile(-1,-1);
}

Tile::Tile(int i, int j)
{
	lastAscii = NULL;
	objects = new Objects();
	_flags.visited = NO;
	_flags.passable  = YES;
	_flags.transparent = YES;
	Position = Point(i,j);
    height = 0;
}

Tile::~Tile()
{
    delete objects;
    lastAscii = NULL;
    parent = NULL;
}

void Tile::addObject(Object *object)
{
    Object *existing = NULL;
    
    if(object->_flags.stackable)
    {
        foreachp(Objects, o, objects)
        {
            Object *test = (*o);
            if(test->name.compare(object->name) != 0)
            {
                // same name
                existing = test;
                break;
            }
        }
    }
    
    if(existing)
    {
        existing->count += object->count; 
    }
    else 
    {
        object->setParent(this);
        objects->push_back(object);
        
        if(object->_flags.passable==NO)
            _flags.passable = NO;
		
        if(object->_flags.transparent==NO)
            _flags.transparent = NO;
    }
}

void Tile::addLiquid(Object *object)
{
    object->setParent(this);
//    Objects::reverse_iterator it = objects->rbegin();
    
    bool insert = objects->size() > 1;
    Object *tmp;
    if(insert)
    {
        tmp = objects->back();
        objects->pop_back();
    }
    
    objects->push_back(object);
    if(insert)
        objects->push_back(tmp);
	
	if(object->_flags.passable==NO)
		_flags.passable = NO;
		
    if(object->_flags.transparent==NO)
        _flags.transparent = NO;
}

void Tile::removeObject(Object *object)
{
	object->setParent(NULL);
	objects->remove(object);
	
	bool passable = true,transparent = true;
	foreachp(Objects,o,objects)
	{
		if((*o)->_flags.passable==NO)
			passable = false;
		
		if((*o)->_flags.transparent==NO)
			transparent = false;
	}
	_flags.passable = passable ? YES : NO ;
	_flags.transparent = transparent ? YES : NO ;
}

Ascii *Tile::getTerainAscii(bool visible)
{
    Ascii *terrain = NULL;
    rforeachp(Objects,o,objects)
    {
        Object *object = (*o);
        terrain = object->getAscii();
        if(object->terrain())
            break;
    }
    return terrain;
}

Objects *Tile::getObjects()
{
    return objects;
}

Ascii *Tile::getTopAscii(bool visible)
{
	Ascii *ascii = NULL;
	if(visible)
	{
		if(objects->size() > 0)
        {   
            Object *object = objects->back();
			ascii = object->getAscii();

            if(ascii->backgroundInherits)
            {
                Colour colour;
                foreachp(Objects,o,objects)
                {
                    //we want to find the last object that doesn't have a clear background colour
                    //this is an over simplification.. Alpha is effectively On or Off, not continuous
                    
                    if ((*o)->getAscii()->backgroundInherits != true) {
                        colour = (*o)->getAscii()->Background;
                    }
                }
                
                //Ascii *low = objects->front()->getAscii();
                ascii->Background = colour;//low->Background;
            }
        }
		_flags.visited = YES;
		lastAscii = ascii;
	}
	else if(_flags.visited)
	{
		ascii = lastAscii;
	}
	
	return ascii;
}

std::string Tile::description()
{
    int i=0;
    std::string desc = "";
    rforeachp(Objects,o,objects)
    {
        Object *object = (*o);
        Monster *monster = dynamic_cast<Monster *>(object);
        if(monster != NULL)
        {
            desc += (*o)->description + " are standing";
        }
        else
        {
            if(object->liquid())
                desc += "in " + (*o)->description;
            else
                desc += "on " + (*o)->description;
            if(object->terrain())
                break; // don't describe under the terrain
        }
        
        i++;
        if(i!=objects->size())
            desc += " ";
    }
    return desc;
}

Object *Tile::getTopObject()
{
    if(objects->size() > 0)
        return objects->back();
    else
        return NULL;
}

void Tile::update(Speed updateSpeed, int turnNumber)
{
    //Probably better to copy the array before we run update.
    Objects objsToMove = *objects;
    
    foreach(Objects,o,objsToMove)
    {
        Object *object = (*o);
        //int count = objects->size();
        //o++;
        if (object != NULL) {
            object->update(updateSpeed, turnNumber);
        }
//        else {
//            break;
//        }
//        if (count != objects->size()) {
//            return;
//        }
    }
}
