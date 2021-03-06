//
//  Inventory.h
//  Roguelike
//
//  Created by Daniel Brooker on 14/03/12.
//  Copyright (c) 2012 Nocturnal Code Limited. All rights reserved.
//

#ifndef INVENTORY_INCLUDE_H
#define INVENTORY_INCLUDE_H

#include "Menu.h"
#include "Player.h"

class Inventory : public Menu
{
    Label *groundLabel;
    
    Player *player;
    
    std::string describeObject(Object *object);
    
    void dropSelected();
    void pickupSelected();
    void equipSelected();
    void unequipSelected();
    void consumeSelected();
    Object *getSelectedObject();
    
public:
    Inventory(Rect rect,Player *player);
    
    virtual int numberOfItems();
    virtual Objects groundItems();
    virtual void didSelectItem(int index);
    
    virtual void open();
    virtual void close();
    virtual bool wantsEvents();
    
    virtual bool handleEvents(SDL_Event *event);
};


#endif /* INVENTORY_INCLUDE_H */
