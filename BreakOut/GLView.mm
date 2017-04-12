//
//  DanmuView.m
//  danmu
//
//  Created by newworld on 16/10/1.
//  Copyright © 2016年 siyuxing. All rights reserved.
//

#import "GLView.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <GLKit/GLKit.h>
#include "GameEngine/Game.hpp"
//#include "Game.hpp"


//typedef struct {
//    float Position[3];
//    float Color[4];
//    float TexCoord[2];
//}Vertex;
//
//static const Vertex Vertices[] = {
//    {{1.0, 0.0, 0.0}, {1, 1, 1, 1}, {1, 1}},
//    {{1.0, 1.0, 0.0}, {1, 1, 1, 1}, {1, 0}},
//    {{0.0, 1.0, 0.0}, {1, 1, 1, 1}, {0, 0}},
//    {{0.0, 0.0, 0.0}, {1, 1,1, 1}, {0, 1}},
//};
//
//static const GLubyte Indices[] = {
//    1, 0, 2, 3
//};




@interface GLView ()
{
    CAEAGLLayer *eaglLayer;
    EAGLContext *glcontext;
    
    CADisplayLink *displayLink;
    
    
  
    //buffer
    GLuint VBO;
    GLuint IBO;
    GLuint colorRenderBuffer;
    
    size_t backingWidth;
    size_t backingHeight;
    
    Game* breakout;
    
    
}

@end


@implementation GLView


- (instancetype) initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setup];
    }
    
    return self;
}

- (instancetype) initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder]) {
        [self setup];
    }
    
    return self;
}


- (void) dealloc
{
    if (colorRenderBuffer) {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
    }
    if (framebuffer) {
        glDeleteBuffers(1, &framebuffer);
    }
    if (IBO) {
        glDeleteBuffers(1, &IBO);
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
    }
    if ([EAGLContext currentContext] == glcontext){
        [EAGLContext setCurrentContext:nil];
    }
}



#pragma mark - setup

- (void) setup
{
    [self setupLayer];
    [self setupContext];
    [self setupRenderBuffer];
    [self setupFrameBuffer];
    [self setupGame];
    [self setupGestureRecognizer];
    [self setupDisplayLink];
}

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void) setupLayer
{
    eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = YES;
}

- (void) setupContext
{
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;
    glcontext = [[EAGLContext alloc] initWithAPI:api];
    [EAGLContext setCurrentContext:glcontext];
}

- (void) setupGame
{
    CGFloat width =  CGRectGetWidth([UIScreen mainScreen].bounds) ;
    CGFloat height = CGRectGetHeight([UIScreen mainScreen].bounds);
    
    breakout = new Game(width,height);
    
}

- (void) setupRenderBuffer
{
    glGenRenderbuffers(1, &colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    [glcontext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];

}

- (void) setupFrameBuffer
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, colorRenderBuffer);
}

- (void) setupGestureRecognizer
{
    UIPanGestureRecognizer *pgr = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
    UITapGestureRecognizer *tgr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap:)];
    
    [self addGestureRecognizer:pgr];
    [self addGestureRecognizer:tgr];
}


- (void) setupDisplayLink
{
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(display:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}



#pragma mark - refresh

- (void) display:(CADisplayLink *)displayLink
{
    [self _update:displayLink];
    [self _display:displayLink];
}



- (void) _update:(CADisplayLink *)displayLink
{
    breakout->Update(displayLink.duration);
}

- (void) _display:(CADisplayLink *)displayLink
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    breakout->Render(displayLink.duration);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    [glcontext presentRenderbuffer:GL_RENDERBUFFER];
    
}

#pragma mark - gr

- (void) pan:(UIPanGestureRecognizer *)pgr
{
    CGPoint translate = [pgr translationInView:self];
    NSLog(@"translate x is %f, y is %f  %s",translate.x,translate.y,__FUNCTION__);
}

- (void) tap:(UITapGestureRecognizer *)tgr
{
    CGPoint location = [tgr locationInView:self];
    NSLog(@"location x is %f, y is %f  %s",location.x,location.y,__FUNCTION__);
    breakout->shoot();
}






/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code
 }
 */

@end
