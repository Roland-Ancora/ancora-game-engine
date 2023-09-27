#version 130


in vec2 TexCoords;

out vec4 FragColor;



uniform sampler2D text;
uniform vec3 textColor;



void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, gl_TexCoord[0].st).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}
