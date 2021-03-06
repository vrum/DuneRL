/*
 *  World.h
 *  
 *
 *  Created by Daniel Brooker on 29/03/08.
 *  Copyright 2008 quantoaduro.com. All rights reserved.
 *
 */

#ifndef WORLD_H_INC
#define WORLD_H_INC  

#include <string>
#include "SDL_OpenGL.h"
#include "Ascii.h"

#include "Random.h"
#include <math.h>
#include "Monster.h"
#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif
#include "Display.h"
#include "Armoury.h"

class Map;

#include <list>
#include <map>

typedef enum {
    ArrowKeys,
    VIKeys,
    KeysCount
}Keys;

typedef struct {
	int count;
	Speed *order;
	int orderCount;
    int playerCount;
} TurnInfo;

class World : public Display
{
    protected:
        Display *parent;
        Map		*map;
		TurnInfo turn;
        Keys movementKeys;
    
        void doUpdateWorld();
		virtual void updateWorld();

    public:
        World();
		
        Armoury *armoury;
		KeyMap	keyPress;

		virtual	void setParent(Display *parent);
		virtual Display* getParent();
		virtual Rect getFrame();
        virtual void display(float *texCoordinates, float *colCoordinates, float *bgColCoordinates);
		virtual bool handleEvents(SDL_Event *event);
		
        int getTurn();
        void playerEndedTurn();
    
        void setMap(Map *map);
        Map *getMap();
};

#endif /* WORLD_H_INC */
