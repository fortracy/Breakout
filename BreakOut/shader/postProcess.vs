//#version 330 core
attribute vec4 vertex; // <vec2 position, vec2 texCoords>

varying vec2 TexCoords;

uniform bool  chaos;
uniform bool  confuse;
uniform bool  shake;
uniform float time;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    vec2 texture1 = vertex.zw;
    TexCoords = texture1;

//    if(chaos)
//    {
//        float strength = 0.3;
//        vec2 pos = vec2(texture1.x + sin(time) * strength, texture1.y + cos(time) * strength);
//        TexCoords = pos;
//    }
//    else if(confuse)
//    {
//        TexCoords = vec2(1.0 - texture1.x, 1.0 - texture1.y);
//    }
//    else
//    {
//        TexCoords = texture1;
//    }
//    if (shake)
//    {
//        float strength = 0.01;
//        gl_Position.x += cos(time * 10.0) * strength;
//        gl_Position.y += cos(time * 15.0) * strength;
//    }
}
