#version 430


layout (location=0) in vec3 VertexPos;

uniform mat4 MVP_matrix;

out vec3 TexturePos;



void main()
{
	TexturePos = vec3(VertexPos.x, VertexPos.y, VertexPos.z);
	gl_Position = MVP_matrix * vec4(VertexPos.x, VertexPos.y, VertexPos.z, 1.0f);
}
