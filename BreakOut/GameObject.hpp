//
//  GameObject.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/24.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include <GLKit/GLKit.h>
#include <OpenGLES/gltypes.h>
#include "Texture2D.hpp"
#include "SpriteRender.hpp"

class GameObject {
    
    
public:
    GLKVector2 Position,Size,Velocity;
    GLKVector3 Color;
    GLfloat Rotation;
    GLboolean isSolid;
    GLboolean Destroyed;
    Texture2D Sprite;
    GameObject();
    GameObject(GLKVector2 pos, GLKVector2 size, Texture2D sprite, GLKVector3 color = GLKVector3Make(1.0, 1.0, 1.0), GLKVector2 velocity = GLKVector2Make(1.0, 1.0));
    virtual void Draw(SpriteRenderer &renderer);
};




#endif /* GameObject_hpp */
