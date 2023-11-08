#version 430


in vec2 TexturePos;
in vec3 light_intensity;

out vec4 FragColor;



uniform sampler2D main_texture_id;
uniform vec3 object_color;



void main()
{
	FragColor = vec4(light_intensity.xyz, 1.0f) * vec4(object_color.xyz, 1.0f) * texture(main_texture_id, TexturePos.xy);
}
