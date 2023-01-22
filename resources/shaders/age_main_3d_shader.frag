#version 130


in vec3 Color;

out vec4 FragColor;



uniform sampler2D main_texture_id;



void main()
{
	FragColor = vec4(Color.xyz, 1.0f) * texture(main_texture_id, gl_TexCoord[0].st);
}
