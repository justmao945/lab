//
//  WBViewController.m
//  Calculator
//
//  Created by Jianjun on 9/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import "WBViewController.h"

@interface WBViewController ()

// the underlying number
@property (strong) NSDecimalNumber* number;

// the mutable string to hold input data
@property (strong) NSMutableString* text;

@end



@implementation WBViewController

@synthesize display;

- (IBAction) clickNumber:(UIButton*)sender
{
    [_text appendString:sender.titleLabel.text];
    display.text = _text;
}

- (IBAction)clickOperator:(UIButton*)sender
{
    [_text appendString:sender.titleLabel.text];
    display.text = _text;
}

- (IBAction)clickOver
{
}

- (IBAction)clickClear
{
    [_text setString:@""];
}

- (IBAction)clickEqual
{
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    _number = [NSDecimalNumber decimalNumberWithString:@"0"];
    _text = [NSMutableString stringWithString:@""];

    NSLog(@"did load");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
