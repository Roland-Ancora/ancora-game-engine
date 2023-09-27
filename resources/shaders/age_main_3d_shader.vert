#version 140


uniform mat4 MVP_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_matrix;

out vec3 Color;
out vec3 light_intensity;



void main()
{
	vec4 global_light_pos = vec4(0.0f, 20.0f, 0.0f, 1.0f);
	vec3 global_light_color = vec3(1.0f, 1.0f, 1.0f);
	vec3 global_light_min_color = vec3(0.25f, 0.25f, 0.25f);

	mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));
    vec3 normal = normalize(normal_matrix * gl_Normal);
	vec3 vert_position = vec3(model_matrix * vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1.0f));
	vec3 s = normalize(vec3(global_light_pos - vec4(vert_position, 1.0f)));
	light_intensity = global_light_color * max(dot(s, normal), 0.0f) + global_light_min_color;

	Color = gl_Color.xyz;
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_Position = MVP_matrix * vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1.0f);
}
