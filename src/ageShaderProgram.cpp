#include "../include/ageShaderProgram.h"



using namespace age;



const char main_3d_vertex_shader[] = "./resources/shaders/age_main_3d_shader.vert";
const char main_3d_fragment_shader[] = "./resources/shaders/age_main_3d_shader.frag";
const char main_2d_vertex_shader[] = "./resources/shaders/age_main_2d_shader.vert";
const char main_2d_fragment_shader[] = "./resources/shaders/age_main_2d_shader.frag";
const int shader_max_len = 5000;



char* load_shader_as_srting(const char* file_name)
{
	FILE* shader_file;
	fopen_s(&shader_file, file_name, "r");
	if (shader_file == 0) {
		printf("Error: can't load shader file: %s!\n", file_name);
		exit(1);
	}
	char shader_string[shader_max_len];
	char ch = 0;
	int cnt = 0;
	while ((ch = fgetc(shader_file)) != EOF) {
		shader_string[cnt] = ch;
		cnt++;
	}
	shader_string[cnt] = 0;
	fclose(shader_file);
	return shader_string;
}

GLuint create_shader_gl(const char* file_name, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	if (shader == 0)
		printf("Error with creating shader.\n");
	char* basic_vert_shader_string = load_shader_as_srting(file_name);
	const GLchar* basic_vert_shader_string_arr[] = { basic_vert_shader_string };
	glShaderSource(shader, 1, basic_vert_shader_string_arr, NULL);
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, "Vertex shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = new char[logLen];
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			delete[] log;
		}
	}
	return shader;
}

GLuint create_shader_program_gl(GLuint shaders[], int shader_count)
{
	GLuint programHandle = glCreateProgram();
	if (0 == programHandle)
		fprintf(stderr, "Error creating program object.\n");
	for (int i = 0; i < shader_count; i++)
		glAttachShader(programHandle, shaders[i]);
	glLinkProgram(programHandle);
	GLint status_prog_comp;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status_prog_comp);
	if (GL_FALSE == status_prog_comp) {
		fprintf(stderr, "Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = new char[logLen];
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			fprintf(stderr, "Program log: \n%s", log);
			delete[] log;
		}
	}
	return programHandle;
}





ShaderProgram ShaderProgram::create_main_shader_3d_program()
{
	return create_shader_program(main_3d_vertex_shader, main_3d_fragment_shader);
}

ShaderProgram ShaderProgram::create_main_shader_2d_program()
{
	return create_shader_program(main_2d_vertex_shader, main_2d_fragment_shader);
}

ShaderProgram ShaderProgram::create_shader_program(const char* vert_shader_file, const char* frag_shader_file)
{
	ShaderProgram shader_prog;
	GLuint vert_shader = create_shader_gl(vert_shader_file, GL_VERTEX_SHADER);
	GLuint frag_shader = create_shader_gl(frag_shader_file, GL_FRAGMENT_SHADER);
	shader_prog.shader_prog_id = create_shader_program_gl(new GLuint[2]{ vert_shader, frag_shader }, 2);
	shader_prog.MVP_mat_loc = glGetUniformLocation(shader_prog.shader_prog_id, "MVP_matrix");
	shader_prog.MV_mat_loc = glGetUniformLocation(shader_prog.shader_prog_id, "model_view_matrix");
	shader_prog.M_mat_loc = glGetUniformLocation(shader_prog.shader_prog_id, "model_matrix");
	return shader_prog;
}
