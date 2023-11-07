#version 430


in vec3 Color;
in vec2 TexturePos;

out vec4 FragColor;



uniform sampler2D main_texture_id;



void main()
{
	FragColor = vec4(vec4(Color.xyz, 1.0f) * texture(main_texture_id, TexturePos.xy));
}
