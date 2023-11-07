#version 430


out vec4 FragColor;


in vec2 TexPos;


uniform sampler2D text;
uniform vec3 textColor;



void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexPos.xy).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}
