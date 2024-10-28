#version 430


uniform sampler2D main_texture_id;

in vec4 Color;
in vec2 TexturePos;

out vec4 FragColor;



void main()
{
	FragColor = vec4(Color * texture(main_texture_id, TexturePos.xy));
}
