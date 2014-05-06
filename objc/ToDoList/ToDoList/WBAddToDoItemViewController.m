//
//  WBAddToDoItemViewController.m
//  ToDoList
//
//  Created by Jianjun on 6/5/14.
//  Copyright (c) 2014 WorldBeautiful. All rights reserved.
//

#import "WBAddToDoItemViewController.h"

@interface WBAddToDoItemViewController ()
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *doneButton;

@end

@implementation WBAddToDoItemViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if(sender != self.doneButton) {
        return;
    }
    
    if (self.textField.text.length > 0) {
        self.todoItem = [[WBToDoItem alloc]init];
        self.todoItem.itemName = self.textField.text;
        self.todoItem.completed = NO;
    }
}


@end
