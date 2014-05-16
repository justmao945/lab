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
    // all physics bodies must be children of this
    CCPhysicsNode *_physicsNode;
    
    // drag
    CCNode *_mouseJointNode;
    CCPhysicsJoint *_mouseJoint;
    
    // a node to pullback catapultArm
    CCNode *_pullbackNode;
    CCPhysicsJoint *_pullbackJoint;
    
    // catapult related
    CCNode *_catapult;
    CCNode *_catapultArm;
    CCPhysicsJoint *_catapultJoint;

    
    // changed in every level, a sub scene
    CCNode *_levelNode;
    
    // except buttons...
    CCNode *_gameNode;
    
    // about penguins
    CCNode* _currentPenguin;
    CCPhysicsJoint *_penguinCatapultJoint;
}

-(void)didLoadFromCCB
{
    // tell this scene to accept touches
    self.userInteractionEnabled = YES;
    
    CCScene* level = [CCBReader loadAsScene:@"Levels/Level1"];
    [_levelNode addChild:level];
    
    ////visualize physics bodies and joints
    //_physicsNode.debugDraw = YES;
    
    // setup delegate
    _physicsNode.collisionDelegate = self;
    
    
    // arm and catapult should not collide, disable this by using the same collision group
    [_catapultArm.physicsBody setCollisionGroup:_catapult];
    [_catapult.physicsBody setCollisionGroup:_catapult];
    
    // BodyA and anchorA should come from the same one
    // Joint anchor point is not the anchor point of a CCNode, it's a relative position to bottom left of the node.
    // Not relative to the anchor point of the node
    //CGPoint pivotAnchor = ccp(_catapultArm.contentSize.width * _catapultArm.anchorPoint.x, _catapultArm.contentSize.height * _catapultArm.anchorPoint.y);
    //CGPoint pivotAnchor = ccp(10, 10);
    _catapultJoint = [CCPhysicsJoint connectedPivotJointWithBodyA:_catapultArm.physicsBody bodyB:_catapult.physicsBody anchorA:_catapultArm.anchorPointInPoints];
    CCLOG(@"arm anchor: %f %f", _catapultArm.anchorPoint.x, _catapultArm.anchorPoint.y);
    
    // nothing should collide with our pullback node
    _pullbackNode.physicsBody.collisionMask = @[];
    _pullbackJoint = [CCPhysicsJoint connectedSpringJointWithBodyA:_pullbackNode.physicsBody bodyB:_catapultArm.physicsBody anchorA:ccp(0, 0) anchorB:ccp(34, 138) restLength:60 stiffness:500 damping:40];
    
    // deactive the collision
    _mouseJointNode.physicsBody.collisionMask = @[];
}


-(void)retry
{
    [[CCDirector sharedDirector] replaceScene:[CCBReader loadAsScene:@"Gameplay"]];
}


-(void)touchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
//    [self launchPenguins];
    // get location from _gameNode is because everything is contained in it.
    CGPoint touchLocation = [touch locationInNode:_gameNode];
    if (CGRectContainsPoint(_catapultArm.boundingBox, touchLocation)) {
        _mouseJointNode.position = touchLocation;
        _mouseJoint = [CCPhysicsJoint connectedSpringJointWithBodyA:_mouseJointNode.physicsBody bodyB:_catapultArm.physicsBody anchorA:ccp(0, 0) anchorB:ccp(34, 138) restLength:0 stiffness:3000 damping:150];
        
        _currentPenguin = [CCBReader load:@"Penguin"];
        // 34, 138 is a point in arm, need to get its position in world, and then covert to node position in physicsNode, as the penguin is a child of it.
        CGPoint penguinPosition = [_catapultArm convertToWorldSpace:ccp(34, 138)];
        _currentPenguin.position = [_physicsNode convertToNodeSpace:penguinPosition];
        [_physicsNode addChild:_currentPenguin];
        _currentPenguin.physicsBody.allowsRotation = NO;
        _penguinCatapultJoint = [CCPhysicsJoint connectedPivotJointWithBodyA:_currentPenguin.physicsBody bodyB:_catapultArm.physicsBody anchorA:_currentPenguin.anchorPointInPoints];
    }
    CCLOG(@"touch began");
}

-(void)touchMoved:(UITouch *)touch withEvent:(UIEvent *)event
{
    CGPoint touchLocation = [touch locationInNode:_gameNode];
    _mouseJointNode.position = touchLocation;
    CCLOG(@"touch moved");
}

-(void)touchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
    [self releaseCatapult];
    CCLOG(@"touch ended");
}

-(void)touchCancelled:(UITouch *)touch withEvent:(UIEvent *)event
{
    [self releaseCatapult];
    CCLOG(@"touch cancelled");
}

-(void)releaseCatapult
{
    if (_mouseJoint != nil) {
        [_mouseJoint invalidate];
        _mouseJoint = nil;
        
        [_penguinCatapultJoint invalidate];
        _penguinCatapultJoint = nil;
        
        _currentPenguin.physicsBody.allowsRotation = YES;
        
        CCActionFollow *follow = [CCActionFollow actionWithTarget:_currentPenguin worldBoundary:_gameNode.boundingBox];
        [_gameNode runAction:follow];
    }
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
    CCActionFollow *follow = [CCActionFollow actionWithTarget:penguin worldBoundary:_gameNode.boundingBox];
    [_gameNode runAction:follow];
}

-(void)ccPhysicsCollisionPostSolve:(CCPhysicsCollisionPair *)pair seal:(CCNode *)nodeA wildcard:(CCNode *)nodeB
{
    CCLOG(@"Something collided with a seal!");
    float energy = [pair totalKineticEnergy];
    if (energy > 5000) {
        [self sealRemoved:nodeA];
    }
}


-(void) sealRemoved:(CCNode*)seal
{
    CCParticleSystem* explosion = (CCParticleSystem*)[CCBReader load:@"SealExplosion"];
    explosion.autoRemoveOnFinish = YES;
    explosion.position = seal.position;
    [seal.parent addChild:explosion];
    [seal removeFromParent];
}

@end