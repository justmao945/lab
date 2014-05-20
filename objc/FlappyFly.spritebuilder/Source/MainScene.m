//
//  MainScene.m
//  PROJECTNAME
//
//  Created by Viktor on 10/10/13.
//  Copyright (c) 2013 Apportable. All rights reserved.
//

#import "MainScene.h"

static const CGFloat scrollSpeed = 80.f;

@implementation MainScene
{
    CCSprite *_hero;
    CCPhysicsNode* _physicsNode;
    CCNode *_ground1;
    CCNode *_ground2;
    NSArray *_grounds;
    NSTimeInterval _sinceTouch;
}

-(void)didLoadFromCCB
{
    self.userInteractionEnabled = YES;
    _grounds = @[_ground1, _ground2];
}


-(void)touchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
    // make bird upward
    [_hero.physicsBody applyImpulse:ccp(0.f, 400.f)];
    // turn bird
    [_hero.physicsBody applyAngularImpulse:10000.f];
    
    // record touch interval
    _sinceTouch = 0;
}


-(void)update:(CCTime)delta
{
    // move hero to right, relative to its parent the physics node
    _hero.position = ccp(_hero.position.x + scrollSpeed * delta, _hero.position.y);
    // move physics node to left with the same speed of moving hero...
    // That makes the hero have a static position x.
    _physicsNode.position = ccp(_physicsNode.position.x - scrollSpeed * delta, _physicsNode.position.y);
    
    // make endless ground
    for (CCNode* ground in _grounds) {
        // ground position is relative to physicsNode
        CGPoint worldPosition = [_physicsNode convertToWorldSpace:ground.position];
        CGPoint screenPosition = [self convertToNodeSpace:worldPosition];
        if (screenPosition.x < -ground.contentSize.width) {
            ground.position = ccp(ground.position.x + 2 * ground.contentSize.width, ground.position.y);
        }
    }
    
    // limit speed of +y direction, but not -y. Also keeps x not changed.
    float yVelocity = clampf(_hero.physicsBody.velocity.y, -MAXFLOAT, 200.f);
    _hero.physicsBody.velocity = ccp(0, yVelocity);

    // limit rotation angular
    _hero.rotation = clampf(_hero.rotation, -30.f, 90.f);
    if (_hero.physicsBody.allowsRotation) {
        float angularVelocity = clampf(_hero.physicsBody.angularVelocity, -2.f, 1.f);
        _hero.physicsBody.angularVelocity = angularVelocity;
    }
    
    _sinceTouch += delta;
    if (_sinceTouch > 0.5) {
        [_hero.physicsBody applyAngularImpulse:-40000*delta];
    }
}

@end
