//
//  SpiceMadness.h
//  Roguelike
//
//  Created by Daniel Brooker on 17/03/12.
//  Copyright (c) 2012 Nocturnal Code Limited. All rights reserved.
//

#ifndef SPICEMADNESS_INCLUDE_H
#define SPICEMADNESS_INCLUDE_H

#include "Effect.h"
#include "Object.h"
#include "LightFilterSpiceMadness.h"

class SpiceMadness : public Effect
{
    bool completeMadness;
    LightFilterSpiceMadness *madnessFilter;
public:
    SpiceMadness(int duration, Object *object);
    ~SpiceMadness();
    virtual void update();
    virtual void onAdded();
    virtual void onRemoved();
    virtual void onFinished();
    
    virtual void addDuration(int d);
    bool isMad();
};

#endif /* SPICEMADNESS_INCLUDE_H */
