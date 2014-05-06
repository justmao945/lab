//
//  main.m
//  prog1
//
//  Created by Jianjun on 30/4/14.
//  Copyright (c) 2014 Jianjun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Fraction.h"

@interface ClassA : NSObject
{
    int x;
}

@end

int main (int argc, const char* argv[]) {
        
    @autoreleasepool {
        Fraction *frac = [[Fraction alloc] init];
        [frac setNumerator: 1];
        [frac setDenominator: 3];
        NSLog(@"The value of frac is:");
        [frac print];
    }

    return 0;
}
