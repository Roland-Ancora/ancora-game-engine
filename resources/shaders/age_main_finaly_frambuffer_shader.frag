#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;

void main()
{
    float exposure = 1.0f;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    FragColor = vec4(mapped, 1.0);
}
