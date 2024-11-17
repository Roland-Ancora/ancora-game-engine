#include "ageShaderProgram.h"



using namespace age;



char* load_shader_as_srting(const char* file_name)
{
	FILE* shader_file;
	fopen_s(&shader_file, file_name, "r");
	if (shader_file == 0) {
		printf("AGE::ERROR::SHADERPROGRAM::OPEN:: Can't open shader file: %s!\n", file_name);
		exit(AGE_SHADER_OPEN_ERROR);
	}
	char shader_string[AGE_SHADER_PROGRAM_MAX_LENGHT];
	char ch = 0;
	int cnt = 0;
	while ((ch = static_cast<char>(fgetc(shader_file))) != EOF) {
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
		printf("AGE::ERROR::SHADERPROGRAM::CREATE:: Can't create shader. [%s]\n", file_name);
	char* basic_vert_shader_string = load_shader_as_srting(file_name);
	const GLchar* basic_vert_shader_string_arr[] = { basic_vert_shader_string };
	glShaderSource(shader, 1, basic_vert_shader_string_arr, NULL);
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		fprintf(stderr, "AGE::ERROR::SHADERPROGRAM::CREATE:: Shader compilation failed. [%s]\n", file_name);
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
		fprintf(stderr, "AGE::ERROR::SHADERPROGRAM::CREATE:: Can't create program object.\n");
	for (int i = 0; i < shader_count; i++)
		glAttachShader(programHandle, shaders[i]);
	glLinkProgram(programHandle);
	GLint status_prog_comp;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status_prog_comp);
	if (GL_FALSE == status_prog_comp) {
		fprintf(stderr, "AGE::ERROR::SHADERPROGRAM::CREATE:: Failed to link shader program!\n");
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





ShaderProgram ShaderProgram::create_shader_program(const char* vert_shader_file, const char* frag_shader_file)
{
	ShaderProgram shader_prog;
	GLuint vert_shader = create_shader_gl(vert_shader_file, GL_VERTEX_SHADER);
	GLuint frag_shader = create_shader_gl(frag_shader_file, GL_FRAGMENT_SHADER);
	shader_prog.shader_prog_id = create_shader_program_gl(new GLuint[2]{ vert_shader, frag_shader }, 2);
	shader_prog.MVP_mat_loc = glGetUniformLocation(shader_prog.shader_prog_id, "MVP_matrix");
	shader_prog.M_mat_loc = glGetUniformLocation(shader_prog.shader_prog_id, "model_matrix");
	return shader_prog;
}
