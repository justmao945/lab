//
//  Tile.h
//  2048
//
//  Created by Jianjun on 21/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "CCNode.h"

@interface Tile : CCNode

// index in grid
@property (nonatomic, assign) int x, y;

// load tile and generate a random value, 2 or 4
+(instancetype)load;

// load tile and generate value with the given one
+(instancetype)loadWithValue:(int) value;

// value getter and setter
-(int)value;
-(void)setValue:(int)value;

@end
