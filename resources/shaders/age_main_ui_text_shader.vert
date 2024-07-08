#version 430


layout (location=0) in vec4 TextData;

uniform mat4 MVP_matrix;
uniform mat4 model_matrix;
uniform float z_val;

out vec2 TexPos;



void main()
{
    TexPos = TextData.zw;
    gl_Position = model_matrix * vec4(z_val, TextData.y, TextData.x, 1.0);
}
