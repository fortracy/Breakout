//
//  BallObject.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/24.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef BallObject_hpp
#define BallObject_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include <GLKit/GLKit.h>
#include "Texture2D.hpp"


class BallObject : public GameObject
{
public:
    // Ball state
    GLfloat   Radius;
    GLboolean Stuck;
    GLboolean Sticky, PassThrough;


    BallObject();
    BallObject(GLKVector2 pos, GLfloat radius, GLKVector2 velocity, Texture2D sprite);
    
    GLKVector2 Move(GLfloat dt, GLuint window_width);
    void  Reset(GLKVector2 position, GLKVector2 velocity);
};

#endif /* BallObject_hpp */
