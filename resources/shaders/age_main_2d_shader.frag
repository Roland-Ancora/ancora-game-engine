#version 430


uniform sampler2D main_texture_id;

in vec2 TexturePos;

out vec4 FragColor;



void main() {
	FragColor = texture(main_texture_id, TexturePos.xy);
}
