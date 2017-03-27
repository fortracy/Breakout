//
//  TextRender.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/24.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef TextRender_hpp
#define TextRender_hpp

#include <stdio.h>
#include <map>
#include <OpenGLES/gltypes.h>
#include "Shader.hpp"
#include <GLKit/GLKMathTypes.h>


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    GLKVector2 Size;    // Size of glyph
    GLKVector2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    // Holds a list of pre-compiled Characters
    std::map<GLchar, Character> Characters;
    // Shader used for text rendering
    Shader TextShader;
    // Constructor
    TextRenderer(GLuint width, GLuint height);
    // Pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint fontSize);
    // Renders a string of text using the precompiled list of characters
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, GLKVector3 color = GLKVector3Make(1.0f, 1.0f, 1.0f));
private:
    // Render state
    GLuint VAO, VBO;
};



#endif /* TextRender_hpp */
