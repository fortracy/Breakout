//
//  GameLevel.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <vector>
#include <OpenGLES/gltypes.h>
#include "GameObject.hpp"
#include "SpriteRender.hpp"




class GameLevel
{
    public:
    std::vector<GameObject> bricks;
    GameLevel(){};
    void Load(const GLchar *file, GLuint levelWidth,GLuint levelHeight);
    void draw(SpriteRenderer &render);
    GLboolean isCompleted();
    private:
    void init(std::vector<std::vector<GLuint>> tileData,GLuint levelWidth,GLuint levelHeight);
};



#endif /* GameLevel_hpp */
