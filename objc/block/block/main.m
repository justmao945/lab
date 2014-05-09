//
//  main.m
//  block
//
//  Created by Jianjun on 9/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        // insert code here...
        NSLog(@"Hello, World!");
        
        ^(){
            NSLog(@"Hello Block");
        }();
    }
    return 0;
}

