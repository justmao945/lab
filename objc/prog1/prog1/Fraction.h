//
//  Fraction.h
//  prog1
//
//  Created by Jianjun on 30/4/14.
//  Copyright (c) 2014 Jianjun. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Fraction : NSObject
@property int numerator, denominator;
+(Fraction*)alloc;

-(void) print;
-(double) convertToNum;
@end

