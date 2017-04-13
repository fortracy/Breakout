//#version 330 core
precision mediump float;

varying  vec2  TexCoords;

uniform sampler2D scene;
uniform vec2      offsets[9];
uniform float     edge_kernel[9];
uniform float     blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
    vec4 color = vec4(0.0);
    vec3 sample[9];
    // sample from texture offsets if using convolution matrix
    if(chaos || shake)
        for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture2D(scene, TexCoords.st + offsets[i]));
    // process effects
    if(chaos)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(sample[i] * edge_kernel[i], 0.0);
        color.a = 1.0;
    }
    else if(confuse)
    {
        color = vec4(1.0 - texture2D(scene, TexCoords).rgb, 1.0);
    }
    else if(shake)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(sample[i] * blur_kernel[i], 0.0);
        color.a = 1.0;
    }
    else
    {
        color =  texture2D(scene, TexCoords);
    }
    
    gl_FragColor = color;
}
