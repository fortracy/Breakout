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
    GLKView *_kView;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    CGFloat width = CGRectGetWidth([UIScreen mainScreen].bounds);
    CGFloat height = CGRectGetHeight([UIScreen mainScreen].bounds);
    
    
    
    UIView *view = self.view;
    if ([view isKindOfClass:[GLKView class]]) {
        GLKView *kView = (GLKView *)view;
        self.delegate = self;
        _kView = kView;
        
        // Create an OpenGL ES context and assign it to the view loaded from storyboard
        _kView.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        [EAGLContext setCurrentContext:_kView.context];
        
        // Set animation frame rate
        self.preferredFramesPerSecond = 60;
    }
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
