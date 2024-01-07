#version 430


uniform sampler2D main_texture_id;
uniform vec3 object_color;

in vec2 TexturePos;

out vec4 FragColor;



void main() {
	FragColor = vec4(object_color.xyz, 1.0f) * texture(main_texture_id, TexturePos.xy);
}
