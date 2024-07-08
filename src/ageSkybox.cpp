#include "../include/ageSkybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include <../depends/stb_image.h>



using namespace age;



ShaderProgram Skybox::skybox_shader;
GLuint Skybox::skybox_shader_texture_loc;
GLuint Skybox::vao;
GLuint Skybox::vbo;
glm::vec3 Skybox::skybox_verticies[] = {
		{-1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f}
};

Skybox::Skybox()
{
	skybox_shader = ShaderProgram::create_shader_program(AGE_DEFAULT_3D_SKYBOX_VERTEX_SHADER, AGE_DEFAULT_3D_SKYBOX_FRAGMENT_SHADER);
	skybox_shader_texture_loc = skybox_shader.get_uniform_location("skybox_texture");
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_verticies), skybox_verticies, GL_STATIC_DRAW);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Creates Skybox with textures from file_names array.
// Order of elements in file_names: right
//									left
//									top
//									bottom
//									front
//									back.
Skybox::Skybox(const char* file_names[6])
{
	Skybox();
	load_from_files(file_names);
}

// Order of elements in file_names: right
//									left
//									top
//									bottom
//									front
//									back.
void Skybox::load_from_files(const char* file_names[6])
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	GLubyte* data = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (int i = 0; i < 6; i++) {
		int tex_width, tex_height, tex_channels;
		if (data != 0)
			stbi_image_free(data);
		data = stbi_load(file_names[i], &tex_width, &tex_height, &tex_channels, 0);
		if (data == NULL)
			printf("Error: %s image was not load!\n", file_names[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, tex_width, tex_height,
			0, tex_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::show()
{
	Camera3D::get_active_3d_camera()->use_shader(&skybox_shader);
	glm::mat4 mat = glm::translate(glm::mat4(1), Camera3D::get_active_3d_camera()->get_position());
	Camera3D::get_active_3d_camera()->set_model_matrix(&mat);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(skybox_shader_texture_loc, 0);
	glEnable(GL_FRONT_AND_BACK);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	Camera::get_active_camera()->use_main_shader();
}
