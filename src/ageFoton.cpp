#include "../include/ageFoton.h"



using namespace age;



Foton::Foton()
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	light_space_matrix_loc = glGetUniformLocation(depth_sg.get_shader_program_id(), "lightSpaceMatrix");
}

void Foton::use()
{
	Camera3D::get_active_3d_camera()->use_shader(&depth_sg);
	global_light_view = glm::lookAt(global_light_pos + Camera3D::get_active_3d_camera()->get_position() + glm::vec3(7, 0, 8.5),
		glm::vec3(0.0f, 0.0f, 0.0f) + Camera3D::get_active_3d_camera()->get_position() + glm::vec3(7, 0, 8.5),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = global_light_proj * global_light_view;
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(light_space_matrix_loc, 1, GL_FALSE, &(lightSpaceMatrix[0][0]));
	glCullFace(GL_FRONT);
}

void Foton::end()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	glViewport(0, 0, Window::get_active_window()->get_width(), Window::get_active_window()->get_height());
	Camera3D::get_active_3d_camera()->use_main_shader();
	GLuint loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "lightSpaceMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(lightSpaceMatrix[0][0]));
	glUniform1i(glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "main_texture_id"), 0);
	glUniform1i(glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "shadowMap"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE0);
}
