#version 130


uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_matrix;

out vec2 TexCoords;



void main()
{
    gl_TexCoord[0]=gl_MultiTexCoord0;
    gl_Position = MVP_matrix * vec4(gl_Vertex.xy, -1.0, 1.0);
    TexCoords = gl_Vertex.zw;
}
