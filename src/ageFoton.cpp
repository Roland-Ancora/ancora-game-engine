#include "ageFoton.h"



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

	GLuint default_color_processing_subroutine = glGetSubroutineIndex(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), GL_FRAGMENT_SHADER, "global_light");
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &default_color_processing_subroutine);

	light_space_matrix_loc = glGetUniformLocation(depth_sg.get_shader_program_id(), "lightSpaceMatrix");
	default_fogging_loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "default_fogging");
	shadow_intensity_loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "shadow_intensity");
	global_light_color_loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "global_light_color");
	global_light_minimal_color_loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "global_light_min_color");
	global_light_pos_loc = glGetUniformLocation(Camera3D::get_active_3d_camera()->get_active_shader()->get_shader_program_id(), "global_light_pos");
	glUniform1f(default_fogging_loc, default_fogging);
	glUniform1f(shadow_intensity_loc, shadow_intensity);
	glUniform3f(global_light_color_loc, global_light_color.x, global_light_color.y, global_light_color.z);
	glUniform3f(global_light_minimal_color_loc, global_light_minimal_color.x, global_light_minimal_color.y, global_light_minimal_color.z);
	glUniform3f(global_light_pos_loc, global_light_pos.x, global_light_pos.y, global_light_pos.z);
}

void Foton::set_default_fogging(float fogging)
{
	default_fogging = fogging;
	glUniform1f(default_fogging_loc, default_fogging);
}

void Foton::set_shadow_intensity(float intensity)
{
	shadow_intensity = intensity;
	glUniform1f(shadow_intensity_loc, shadow_intensity);
}

void Foton::set_global_light_color(glm::vec3 color)
{
	global_light_color = color;
	glUniform3f(global_light_color_loc, global_light_color.x, global_light_color.y, global_light_color.z);
}

void Foton::set_global_light_minimal_color(glm::vec3 color)
{
	global_light_minimal_color = color;
	glUniform3f(global_light_minimal_color_loc, global_light_minimal_color.x, global_light_minimal_color.y, global_light_minimal_color.z);
}

void Foton::use()
{
	Camera3D::get_active_3d_camera()->use_shader(&depth_sg);
	global_light_view = glm::lookAt(global_light_pos + Camera3D::get_active_3d_camera()->get_position(),
		Camera3D::get_active_3d_camera()->get_position() + global_light_center,
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

void Foton::set_global_light_position(glm::vec3 pos)
{
	global_light_pos = pos;
	glUniform3f(global_light_pos_loc, global_light_pos.x, global_light_pos.y, global_light_pos.z);
}

void Foton::set_global_light_center(glm::vec3 center)
{
	global_light_center = center;
}

void Foton::set_global_light_field_size(float width_x, float width_y)
{
	ortho_width_x = width_x, ortho_width_y = width_y;
	global_light_proj = glm::ortho(-ortho_width_x / 2, ortho_width_x / 2, -ortho_width_y / 2, ortho_width_y / 2, near_plane, far_plane);
}

void Foton::set_global_light_resolution(unsigned int res)
{
	SHADOW_WIDTH = res;
	SHADOW_HEIGHT = res;

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
