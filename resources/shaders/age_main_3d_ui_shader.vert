#version 430


layout (location=0) in vec3 VertexPos;
layout (location=1) in vec2 VertexTexturePos;


uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_matrix;

out vec3 Color;
out vec2 TexturePos;



void main()
{
	Color = vec3(1.0, 1.0, 1.0);
	//gl_TexCoord[0]=gl_MultiTexCoord0;
	TexturePos = VertexTexturePos;
	gl_Position = MVP_matrix * vec4(VertexPos.x, VertexPos.y, VertexPos.z, 1.0f);
}
