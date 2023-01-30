#include "../include/ageCamera.h"



using namespace age;



glm::mat4 Camera::ident_mat = glm::mat4(1);
Camera* Camera::active_camera = 0;

Camera::Camera()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	active_camera = this;
}

void Camera::set_model_matrix(glm::mat4* model_mat)
{
	model_matrix = model_mat;
	MV_matrix = view_matrix * *model_matrix;
	MVP_matrix = projection_matrix * MV_matrix;
	glUniformMatrix4fv(now_active_shader->get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
	glUniformMatrix4fv(now_active_shader->get_MV_matrix_location(), 1, GL_FALSE, &MV_matrix[0][0]);
}





void Camera2D::clear_buffers()
{
	glClear(GL_COLOR_BUFFER_BIT);
}





Camera3D::Camera3D()
{
	glEnable(GL_DEPTH_TEST);
	main_shader_prog = ShaderProgram::create_main_shader_3d_program();
	camera_mode = AGE_CAMERA_PERSPECTIVE;
	projection_matrix = glm::perspective(glm::radians(persp_fov), aspects_ratio, persp_z_near, persp_z_far);
	glUseProgram(main_shader_prog.get_shader_program_id());
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

void Camera3D::calculate_and_use_MVP_matrices()
{
	view_matrix = glm::lookAt(position, position + rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	MV_matrix = view_matrix * *model_matrix;
	MVP_matrix = projection_matrix * MV_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
	glUniformMatrix4fv(main_shader_prog.get_MV_matrix_location(), 1, GL_FALSE, &MV_matrix[0][0]);
}

void Camera3D::calculate_rotation_from_radians()
{
	rotation.x = sin(rotation_radians_xy[0]) * cos(rotation_radians_xy[1]);
	rotation.y = sin(rotation_radians_xy[1]);
	rotation.z = cos(rotation_radians_xy[0]) * cos(rotation_radians_xy[1]);
}

void Camera3D::clear_buffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera3D::set_aspects_ratio(float x, float y)
{
	aspects_ratio = x / y;
	if (camera_mode == AGE_CAMERA_PERSPECTIVE)
		projection_matrix = glm::perspective(glm::radians(persp_fov), aspects_ratio, persp_z_near, persp_z_far);
	else
		projection_matrix = glm::ortho(-aspects_ratio / 2 * ortho_fov, 
										aspects_ratio / 2 * ortho_fov, 
										-0.5f * ortho_fov, 0.5f * ortho_fov, 
										ortho_z_near, ortho_z_far);
	MVP_matrix = projection_matrix * MV_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
	glUniformMatrix4fv(main_shader_prog.get_MV_matrix_location(), 1, GL_FALSE, &MV_matrix[0][0]);
}

void Camera3D::set_position(float x, float y, float z)
{
	position.x += x - position.x;
	position.y += y - position.y;
	position.z += z - position.z;
	calculate_and_use_MVP_matrices();
}

void Camera3D::move(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	calculate_and_use_MVP_matrices();
}

void Camera3D::set_rotation(float x, float y)
{
	rotation_radians_xy.x = x;
	rotation_radians_xy.y = y;
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

void Camera3D::rotate(float x, float y)
{
	rotation_radians_xy.x += x;
	rotation_radians_xy.y += y;
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

void Camera3D::set_orthographic()
{
	camera_mode = AGE_CAMERA_ORTHOGRAPHIC;
	projection_matrix = glm::ortho(-(aspects_ratio / 2) * ortho_fov,
									aspects_ratio / 2 * ortho_fov,
									-0.5f * ortho_fov,
									0.5f * ortho_fov, ortho_z_near, ortho_z_far);
	MVP_matrix = projection_matrix * MV_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

void Camera3D::set_orthographic(float fov)
{
	ortho_fov = fov;
	set_orthographic();
}

void Camera3D::set_orthographic(float z_near, float z_far)
{
	ortho_z_near = z_near;
	ortho_z_far = z_far;
	set_orthographic();
}

void Camera3D::set_perspective()
{
	camera_mode = AGE_CAMERA_PERSPECTIVE;
	projection_matrix = glm::perspective(glm::radians(persp_fov), aspects_ratio, persp_z_near, persp_z_far);
	MVP_matrix = projection_matrix * MV_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

void Camera3D::set_perspective(float fov)
{
	persp_fov = fov;
	set_perspective();
}

void Camera3D::set_perspective(float z_near, float z_far)
{
	persp_z_near = z_near;
	persp_z_far = z_far;
	set_perspective();
}
