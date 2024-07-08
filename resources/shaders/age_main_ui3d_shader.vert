#version 430


layout (location=0) in vec4 VertexData;

uniform mat4 MVP_matrix;
uniform mat4 model_matrix;

out vec2 TexturePos;



void main()
{
	TexturePos = vec2(VertexData.zw);
	gl_Position = MVP_matrix * vec4(VertexData.x, VertexData.y, 0.0f, 1.0f);
}
