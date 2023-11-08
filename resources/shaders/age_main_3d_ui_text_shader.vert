#version 430


layout (location=0) in vec4 TextData;


uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_matrix;


out vec2 TexPos;



void main()
{
    TexPos = TextData.zw;
    gl_Position = MVP_matrix * vec4(TextData.xy, -1.0, 1.0);
}
