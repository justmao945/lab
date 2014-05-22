//
//  Tile.m
//  2048
//
//  Created by Jianjun on 21/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "Tile.h"

@implementation Tile
{
    CCNodeColor *_backgroundNode;
    CCLabelTTF *_valueLabel;
    int _value;
}

-(int)value
{
    return _value;
}

-(void)setValue:(int)value
{
    _value = value;
    _valueLabel.string  = [NSString stringWithFormat:@"%d", _value];
}


+(instancetype)loadWithIndex:(Index *)index
{
    Tile* tile = (Tile*)[CCBReader load:@"Tile"];
    int value = 2;
    if (arc4random() % 4 == 0) {
        value = 4;
    }
    [tile setValue:value];
    [tile setIndex:index];
    return tile;
}

+(instancetype)loadWithIndex:(Index *)index andValue:(int)value
{
    Tile* tile = (Tile*)[CCBReader load:@"Tile"];
    [tile setValue:value];
    [tile setIndex:index];
    return tile;
}

@end
