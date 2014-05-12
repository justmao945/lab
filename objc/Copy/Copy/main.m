//
//  main.m
//  Copy
//
//  Created by Jianjun on 10/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WBNeedToCopy : NSObject <NSCopying>

@property NSString *name, *email;

-(instancetype) initWithName:(NSString*)name andEmail:(NSString*)email;

@end


@implementation WBNeedToCopy

-(instancetype) initWithName:(NSString *)name andEmail:(NSString *)email
{
    self = [self init];
    if (self != nil) {
        self.name = name;
        self.email = email;
    }
    return self;
}

@end


int main(int argc, const char * argv[])
{
    @autoreleasepool {
        WBNeedToCopy* ntc = [[WBNeedToCopy alloc] initWithName:@"test" andEmail:@"test@test.com"];
        WBNeedToCopy* nntc = [ntc copy];
        NSLog(@"%@ %@", nntc.name, nntc.email);
        // insert code here...
        NSLog(@"Hello, World!");
    }
    return 0;
}

