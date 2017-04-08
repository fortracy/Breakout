//
//  SpriteRender.cpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#include "SpriteRender.hpp"
#include "Shader.hpp"



SpriteRenderer::SpriteRenderer(const Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1,&this->quadVAO);
}

void SpriteRenderer::DrawSprite(const Texture2D &texture, GLKVector2 position, GLKVector2 size, GLfloat rotate, GLKVector3 color)
{
    GLenum error;
    
    this->shader.Use();
    
    GLKMatrix4 model = GLKMatrix4Identity;
    model = GLKMatrix4Translate(model, position.x, position.y, 0.0);
    model = GLKMatrix4Translate(model, 0.5f*size.x, 0.5f*size.y, 0.0f);
    model = GLKMatrix4Rotate(model, rotate, 0.0f, 0.0f, 1.0f);
    model = GLKMatrix4Translate(model, -0.5f*size.x, -0.5f*size.y, 0.0f);
    model = GLKMatrix4Scale(model, size.x, size.y, 1.0f);

    
    this->shader.SetMatrix4("model", model);
    
    // Render textured quad
    this->shader.SetVector3f("spriteColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    
    glBindVertexArray(this->quadVAO);
    //glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    
    error = glGetError();
    
    
}
static GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};


void SpriteRenderer::initRenderData()
{
    GLuint VBO;
     glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);

    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
