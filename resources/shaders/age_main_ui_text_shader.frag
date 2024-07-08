#version 430


uniform sampler2D text;
uniform vec3 textColor;

out vec4 FragColor;

in vec2 TexPos;



void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexPos.xy).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}
