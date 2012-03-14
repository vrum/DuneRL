//
//  Spice.cpp
//  Roguelike
//
//  Created by Daniel Brooker on 14/03/12.
//  Copyright (c) 2012 Nocturnal Code Limited. All rights reserved.
//

#include "Spice.h"

Spice::Spice()
{
    _flags.stackable = YES;
    _flags.consumable = YES;
    _flags.carryable = YES;
    _flags.liquid = YES;
}