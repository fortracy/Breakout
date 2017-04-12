//
//  SpriteRender.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef SpriteRender_hpp
#define SpriteRender_hpp

#include <stdio.h>
#include <OpenGLES/gltypes.h>
#include <GLKit/GLKit.h>
#include "Texture2D.hpp"
#include <OpenGLES/ES3/gl.h>
#include "Shader.hpp"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(const Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(const Texture2D &texture, GLKVector2 position, GLKVector2 size = {10.0, 10.0}, GLfloat rotate = 0.0f, GLKVector3 color = {1.0f, 1.0f, 1.0f});
private:
    // Render state
    Shader shader;
    GLuint quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};



#endif /* SpriteRender_hpp */
