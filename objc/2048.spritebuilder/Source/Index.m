//
//  Index.m
//  2048
//
//  Created by Jianjun on 22/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "Index.h"

@implementation Index

-(instancetype) initWithX:(int)x andY:(int)y
{
    self = [super init];
    if (self) {
        self.x = x;
        self.y = y;
    }
    return self;
}


@end
