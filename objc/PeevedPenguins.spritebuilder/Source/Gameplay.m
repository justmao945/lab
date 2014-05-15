//
//  Gameplay.m
//  PeevedPenguins
//
//  Created by Jianjun on 14/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "Gameplay.h"

@implementation Gameplay
{
    CCPhysicsNode *_physicsNode;
    CCNode *_catapultArm;
    CCNode *_levelNode;
    CCNode *_gameNode;
}

-(void)retry
{
    [[CCDirector sharedDirector] replaceScene:[CCBReader loadAsScene:@"Gameplay"]];
}

-(void)didLoadFromCCB
{
    // tell this scene to accept touches
    self.userInteractionEnabled = YES;
    
    CCScene* level = [CCBReader loadAsScene:@"Levels/Level1"];
    [_levelNode addChild:level];
}

-(void)touchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
    [self launchPenguins];
}

-(void)launchPenguins
{
    CCNode *penguin = [CCBReader load:@"Penguin"];
    penguin.position = ccpAdd(_catapultArm.position, ccp(16, 50));
    
    [_physicsNode addChild:penguin];
    
    CGPoint launchDirection = ccp(1, 0);
    CGPoint force = ccpMult(launchDirection, 8000);
    [penguin.physicsBody applyForce:force];
    
    // ensure followed object is in visible are when starting
    // reset position to left most when launch
    _gameNode.position = ccp(0,0);
    CCLOG(@"%f %f", self.position.x, self.position.y);
    CCActionFollow *follow = [CCActionFollow actionWithTarget:penguin worldBoundary:_gameNode.boundingBox];
    [_gameNode runAction:follow];
}





@end