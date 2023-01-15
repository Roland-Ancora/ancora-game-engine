#version 130


in vec3 Color;

out vec4 FragColor;



void main()
{
	FragColor = vec4(Color.xyz, 1.0f);
}
