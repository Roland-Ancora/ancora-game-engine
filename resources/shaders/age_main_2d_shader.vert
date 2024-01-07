#version 430


layout (location=0) in vec2 VertexPos;
layout (location=1) in vec2 VertexTexPos;

uniform mat4 MVP_matrix;
uniform mat4 model_matrix;

out vec2 TexturePos;



void main() {
	TexturePos = VertexTexPos;
	gl_Position = MVP_matrix * vec4(VertexPos.x, VertexPos.y, -0.02f, 1.0f);
}
