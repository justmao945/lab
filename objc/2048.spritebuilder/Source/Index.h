//
//  Index.h
//  2048
//
//  Created by Jianjun on 22/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Index : NSObject

@property (nonatomic, assign) int x, y;

-(instancetype) initWithX:(int)x andY:(int)y;

@end