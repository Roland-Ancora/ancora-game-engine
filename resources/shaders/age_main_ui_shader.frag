#version 430


uniform sampler2D main_texture_id;

in vec3 Color;
in vec2 TexturePos;

out vec4 FragColor;



void main()
{
	FragColor = vec4(vec4(Color.xyz, 1.0f) * texture(main_texture_id, TexturePos.xy));
}
