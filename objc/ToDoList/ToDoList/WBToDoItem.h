//
//  WBToDoItem.h
//  ToDoList
//
//  Created by Jianjun on 6/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WBToDoItem : NSObject

@property NSString *itemName;
@property BOOL completed;
@property (readonly) NSDate* creationDate;

@end
