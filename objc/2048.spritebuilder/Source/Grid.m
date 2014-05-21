//
//  Grid.m
//  2048
//
//  Created by Jianjun on 21/5/14.
//  Copyright (c) 2014 Apportable. All rights reserved.
//

#import "Tile.h"
#import "Grid.h"

// a 4x4 grid
static const int GRID_SIZE = 4;

@implementation Grid
{
    // w, h, mh, mv for tile.
    CGFloat _tileWidth, _tileHeight;
    CGFloat _tileMarginHr, _tileMarginVt;
    
    // nil means empty
    Tile* _grid[GRID_SIZE][GRID_SIZE];
    
    // filled after calling setupBg()
    CCNode* _gridBg[GRID_SIZE][GRID_SIZE];
}

-(void) didLoadFromCCB
{
    // prepare
    CCNode* tile = [CCBReader load:@"Tile"];
    _tileWidth = tile.contentSize.width;
    _tileHeight = tile.contentSize.height;
    
    _tileMarginHr = (self.contentSize.width - tile.contentSize.width * GRID_SIZE) / (GRID_SIZE + 1);
    _tileMarginVt = (self.contentSize.height - tile.contentSize.height * GRID_SIZE) / (GRID_SIZE + 1);
    
    // setup independent bg
    [self setupBg];
    
    // load first tile
    [self loadATile];
}


// fill _gridBg
-(void)setupBg
{
    // get width and height
    CCNode* bg = [CCBReader load:@"TileBg"];
    // calculate margin
    CGFloat marginHr = (self.contentSize.width - bg.contentSize.width * GRID_SIZE) / (GRID_SIZE + 1);
    CGFloat marginVt = (self.contentSize.height - bg.contentSize.height * GRID_SIZE) / (GRID_SIZE + 1);
    
    // set grid bg
    CGFloat x = marginHr;
    for (int i = 0; i < GRID_SIZE; ++i) {
        CGFloat y = marginVt;
        for (int j = 0; j < GRID_SIZE; ++j) {
            bg = [CCBReader load:@"TileBg"];
            [bg setPosition:ccp(x, y)];
            [self addChild:bg];
            _gridBg[i][j] = bg;
            y += bg.contentSize.height + marginVt;
        }
        x += bg.contentSize.width + marginHr;
    }
}

// return NO if is full, or YES
// after calling this, loaded tile will be set to correct position
// and x,y of Tile will be set.
-(BOOL)loadATile
{
    // used to generate random empty position
    int map[GRID_SIZE*GRID_SIZE][2];
    int mapSize = 0;
    
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (_grid[i][j] == nil) {
                map[mapSize][0] = i;
                map[mapSize][1] = j;
                mapSize++;
            }
        }
    }
    if (mapSize == 0) {
        return NO;
    }
    
    // empty found
    int ix = arc4random() % mapSize;
    Tile* tile = [Tile load];
    tile.x = map[ix][0];
    tile.y = map[ix][1];
    
    // fill _grid
    _grid[tile.x][tile.y] = tile;
    [tile setPosition:ccp(_tileMarginHr * (tile.x + 1) + _tileWidth * tile.x , _tileMarginVt * (tile.y  + 1) + _tileWidth * tile.y)];
    [self addChild:tile];
    
    return YES;
}

@end
