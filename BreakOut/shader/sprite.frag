//#version 330 core
precision mediump float;
varying vec2 TexCoords;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
   // gl_FragColor = vec4(0.6,0.4,0.3, 1.0);
    gl_FragColor = vec4(spriteColor, 1.0) * texture2D(image, TexCoords);
}
