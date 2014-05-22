//
//  Tile.h
//  2048
//
//  Created by Jianjun on 21/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "CCNode.h"
#import "Index.h"

@interface Tile : CCNode

@property (nonatomic, strong) Index* index;

+(instancetype)loadWithIndex:(Index*)index;

+(instancetype)loadWithIndex:(Index*)index andValue:(int) value;

-(int)value;
-(void)setValue:(int)value;

@end
