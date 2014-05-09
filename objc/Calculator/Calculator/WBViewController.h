//
//  WBViewController.h
//  Calculator
//
//  Created by Jianjun on 9/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WBViewController : UIViewController

// the input and result label display
@property (strong, nonatomic) IBOutlet UILabel* display;
- (IBAction)clickNumber:(UIButton *)sender;
- (IBAction)clickOperator:(UIButton*)sender;
- (IBAction)clickOver;
- (IBAction)clickEqual;
- (IBAction)clickClear;
@end
