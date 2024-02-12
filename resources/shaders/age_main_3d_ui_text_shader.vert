#version 430


layout (location=0) in vec4 TextData;

uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_matrix;
uniform float z_val;

out vec2 TexPos;



void main()
{
    TexPos = TextData.zw;
    gl_Position = MVP_matrix * vec4(TextData.xy, z_val, 1.0);
}
