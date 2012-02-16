/*
 *  FantasyRL.cpp
 *  Roguelike
 *
 *  Created by Daniel Brooker on 29/04/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "FantasyRL.h"

#include "Stringer.h"
#include "LabelValue.h"
#include "Player.h"

#include "World.h"
#include "Map.h"

FantasyRL::FantasyRL()
{
	init_window();
	init_world();
	
	menuWindow = new ExitMenu(Rect(256-24,128,140+32,64+24));
	
	rootWindow = new Window(Rect(0,0,width,height));
	rootWindow->delegate = world;
	rootWindow->eventDelegate = world;
	rootWindow->texture = sprite;
	rootWindow->getCentreLabel()->setString("FantasyRL");
	//	rootWindow->getCentreLabel()->setColour(Colour::Red());
	rootWindow->getRightLabel()->setString("v0.1");
	
	character = new Window(Rect(16,24,140+32,32+16));
	character->borderStyle = Border_Single;
	character->getLeftLabel()->setString("Draconis");
	character->getLeftLabel()->setColour(Colour::Yellow());
	
	Label *familyLabel = new Label("Static Label");
	familyLabel->setFrame(Rect(16,18,100,40));
	familyLabel->setColour(Colour(1,1,1));
	character->add(familyLabel);
	
    CallBack<Player, std::string> *getHealth = new CallBack<Player, std::string>(player,&Player::hpDescription);
    LabelValue<std::string,Player> *health = new LabelValue<std::string,Player>("",getHealth);
	health->setFrame(Rect(16,18+12,100,40));
	health->setColour(Colour(0,1,0));
	character->add(health);
	
	rootWindow->add(character);
}

FantasyRL::~FantasyRL()
{
}

void FantasyRL::init_world()
{
	int worldSize = 200;
	
	Random::setSeed(42);
	
	sprite = new Sprite(filenameWithFormat("tileset.png") /*"FantasyRL.app/Contents/Resources/tileset.png"*/,16);
    world = new World();
    
    world->setMap(new Map(worldSize));
	
	player = new Player();
	player->setTransparent(false);
	player->setPassable(false);
	//	player->setSight(50);
    
    world->getMap()->addObject(12,12,player);
	world->getMap()->setPlayer(player);
}