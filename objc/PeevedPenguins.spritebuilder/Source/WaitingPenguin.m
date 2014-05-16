//
//  WaitingPenguin.m
//  PeevedPenguins
//
//  Created by Jianjun on 16/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "WaitingPenguin.h"

@implementation WaitingPenguin

-(void)didLoadFromCCB
{
    // generate rand [0,2.0]
    float delay = (arc4random() % 2000) / 1000.f;
    [self performSelector:@selector(startBlinkAndJump) withObject:nil afterDelay:delay];
}

-(void)startBlinkAndJump
{
    // the animation manager of each node is stored in the 'userObject' property

    CCBAnimationManager *am = self.userObject;
    // timelines can be referenced and run by name

    [am runAnimationsForSequenceNamed:@"BlinkAndJump"];
}

@end
