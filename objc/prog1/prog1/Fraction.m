//
//  Fraction.m
//  prog1
//
//  Created by Jianjun on 30/4/14.
//  Copyright (c) 2014 Jianjun. All rights reserved.
//

#import "Fraction.h"

@implementation Fraction
//@synthesize numerator, denominator;

+(Fraction*) alloc
{
    return [super alloc];
}

-(void) print
{
    NSLog(@"%i/%i", _numerator, _denominator);
}

-(double) convertToNum
{
    return 1.0 * _numerator / _denominator;
}

@end
