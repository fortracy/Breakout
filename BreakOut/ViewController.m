//
//  ViewController.m
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#import "ViewController.h"
#include "GameEngine/Game.hpp"

@interface ViewController ()<GLKViewControllerDelegate>
{
    Game *breakout;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    CGFloat width = CGRectGetWidth([UIScreen mainScreen].bounds);
    CGFloat height = CGRectGetHeight([UIScreen mainScreen].bounds);
    
    
    breakout = new Game(width,height);
    // Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    breakout->Render();
}


- (void) glkViewControllerUpdate:(GLKViewController *)controller
{
    NSTimeInterval update = controller.timeSinceLastUpdate;
    breakout->Update(update);
}



@end
