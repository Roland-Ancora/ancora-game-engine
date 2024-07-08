#version 430


uniform sampler2D main_texture_id;
uniform float alpha_channel;

in vec2 TexturePos;

out vec4 FragColor;



void main()
{
	FragColor =  vec4(1.0f, 1.0f, 1.0f, alpha_channel) * texture(main_texture_id, TexturePos.xy);
}
