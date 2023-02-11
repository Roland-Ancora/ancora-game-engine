#version 140


in vec3 Color;
in vec3 light_intensity;

out vec4 FragColor;



uniform sampler2D main_texture_id;



void main()
{
	FragColor = vec4(light_intensity.xyz, 1.0f) * vec4(Color.xyz, 1.0f) * texture(main_texture_id, gl_TexCoord[0].st);
}
