#version 430


uniform samplerCube skybox_texture;

in vec3 TexturePos;

out vec4 FragColor;



void main()
{
	FragColor = texture(skybox_texture, TexturePos);
}
