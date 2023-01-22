#version 130


uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;

out vec3 Color;



void main()
{
	Color = gl_Color.xyz;
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_Position = MVP_matrix * vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1.0f);
}
