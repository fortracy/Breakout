//#version 330 core
attribute vec2 TexCoords;
varying vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
