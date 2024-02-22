#include "../include/ageCamera.h"



using namespace age;



glm::mat4 Camera::ident_mat = glm::mat4(1);
Camera* Camera::active_camera = 0;

Camera::Camera()
{
	if (active_camera != 0)
		exit(AGE_CAMERA_OBJECT_OVERLAPPING_ERROR);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	active_camera = this;
}

// Sets a model matrix, with which the next object will be drawn. Note that every object calls this function before rendering
// and you can't override this action.
void Camera::set_model_matrix(glm::mat4* model_mat)
{
	model_matrix = model_mat;
	MVP_matrix = projection_matrix * view_matrix * *model_mat;
	glUniformMatrix4fv(now_active_shader->get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
	glUniformMatrix4fv(now_active_shader->get_M_matrix_location(), 1, GL_FALSE, &(*model_matrix)[0][0]);
}

// Use the specified shader program.
void Camera::use_shader(ShaderProgram* shader_prog)
{
	now_active_shader = shader_prog;
	glUseProgram(now_active_shader->get_shader_program_id());
	glUniformMatrix4fv(now_active_shader->get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
	glUniformMatrix4fv(now_active_shader->get_M_matrix_location(), 1, GL_FALSE, &(*model_matrix)[0][0]);
}

// Use the default shader program.
void Camera::use_main_shader()
{
	now_active_shader = &main_shader_prog;
	glUseProgram(now_active_shader->get_shader_program_id());
}





Camera2D* Camera2D::active_2d_camera = 0;

Camera2D::Camera2D()
{
	active_2d_camera = this;
	main_shader_prog = ShaderProgram::create_main_shader_2d_program();
	projection_matrix = glm::ortho(0.0f, fov, 0.0f, fov / aspects_ratio, z_near, z_far);
	glUseProgram(main_shader_prog.get_shader_program_id());
	calculate_and_use_MVP_matrices();
}

void Camera2D::calculate_and_use_MVP_matrices()
{
	view_matrix = glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), vector_up);
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

// Clears color buffer. Called every frame.
void Camera2D::clear_buffers()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

// Calculatings and apples new aspects ratio. If you want to get a compress image, your aspects 
//ratio (x / y) must be less than your window aspects ratio, or vice versa for a stretched image. 
// x - width, y - height.
void Camera2D::set_aspects_ratio(float x, float y)
{
	aspects_ratio = x / y;
	projection_matrix = glm::ortho(0.0f, fov, 0.0f, fov / aspects_ratio, z_near, z_far);
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

void Camera2D::set_fov(float new_fov)
{
	fov = new_fov;
	projection_matrix = glm::ortho(0.0f, fov, 0.0f, fov / aspects_ratio, z_near, z_far);
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

void Camera2D::move(float x, float y)
{
	position.x += x;
	position.y += y;
	calculate_and_use_MVP_matrices();
}

void Camera2D::move_x(float x)
{
	position.x += x;
	calculate_and_use_MVP_matrices();
}

void Camera2D::move_y(float y)
{
	position.y += y;
	calculate_and_use_MVP_matrices();
}

void Camera2D::set_position(float x, float y)
{
	position.x += x - position.x;
	position.y += y - position.y;
	calculate_and_use_MVP_matrices();
}

void Camera2D::set_position_x(float x)
{
	position.x += x - position.x;
	calculate_and_use_MVP_matrices();
}

void Camera2D::set_position_y(float y)
{
	position.y += y - position.y;
	calculate_and_use_MVP_matrices();
}





Camera3D* Camera3D::active_3d_camera = 0;

Camera3D::Camera3D()
{
	active_3d_camera = this;
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
	view_matrix = glm::lookAt(position, position + view_point_in_sphere, glm::vec3(0.0f, 1.0f, 0.0f));
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

void Camera3D::calculate_rotation_from_radians()
{
	view_point_in_sphere.x = sin(rotation_radians_xy[0]) * cos(rotation_radians_xy[1]);
	view_point_in_sphere.y = sin(rotation_radians_xy[1]);
	view_point_in_sphere.z = cos(rotation_radians_xy[0]) * cos(rotation_radians_xy[1]);
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
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
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

// Rotates camera in specified values in radians.x value around Y coordinate,
// y value around X coordinate.
void Camera3D::set_rotation(float x, float y)
{
	rotation_radians_xy.x = x;
	rotation_radians_xy.y = y;
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

// Rotates camera by specified values in radians. x value around Y coordinate, 
// y value around X coordinate.
void Camera3D::rotate(float x, float y)
{
	rotation_radians_xy.x += x;
	rotation_radians_xy.y += y;
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

// Sets a camera to orthographic mode.
void Camera3D::set_orthographic()
{
	camera_mode = AGE_CAMERA_ORTHOGRAPHIC;
	projection_matrix = glm::ortho(-(aspects_ratio / 2) * ortho_fov,
									aspects_ratio / 2 * ortho_fov,
									-0.5f * ortho_fov,
									0.5f * ortho_fov, ortho_z_near, ortho_z_far);
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

// Sets a camera to orthographic mode with specified FOV. 
// Default is 15.0. After a change value is saved.
void Camera3D::set_orthographic(float fov)
{
	ortho_fov = fov;
	set_orthographic();
}

// Sets a camera to orthographic mode with specified Z near and Z far. 
// Default is 0.01 and 50.0. After a change values are saved
void Camera3D::set_orthographic(float z_near, float z_far)
{
	ortho_z_near = z_near;
	ortho_z_far = z_far;
	set_orthographic();
}

// Sets a camera to perspective mode.
void Camera3D::set_perspective()
{
	camera_mode = AGE_CAMERA_PERSPECTIVE;
	projection_matrix = glm::perspective(glm::radians(persp_fov), aspects_ratio, persp_z_near, persp_z_far);
	MVP_matrix = projection_matrix * view_matrix * *model_matrix;
	glUniformMatrix4fv(main_shader_prog.get_MVP_matrix_location(), 1, GL_FALSE, &MVP_matrix[0][0]);
}

// Sets a camera to perspective mode with specified FOV. 
// Default is 55.0. After a change value is saved.
void Camera3D::set_perspective(float fov)
{
	persp_fov = fov;
	set_perspective();
}

// Sets a camera to perspective mode with specified Z near and Z far. 
// Default is 0.01 and 50.0. After a change values are saved.
void Camera3D::set_perspective(float z_near, float z_far)
{
	persp_z_near = z_near;
	persp_z_far = z_far;
	set_perspective();
}

// Returns the coordinates of the object point in world coordinates 
// that the pixel displays
glm::vec3 Camera3D::get_pixel_position_in_world_coords(int pixel_pos_x, int pixel_pos_y, int window_height)
{
	int vievport[4];
	GLfloat pixels;
	glGetIntegerv(GL_VIEWPORT, vievport);
	glReadPixels(pixel_pos_x, window_height - pixel_pos_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixels);
	glm::vec3 pr_vec3 = glm::unProject(glm::vec3(pixel_pos_x, window_height - pixel_pos_y, pixels),
		view_matrix,
		projection_matrix,
		glm::vec4(vievport[0], vievport[1], vievport[2], vievport[3]));
	return glm::vec3(pr_vec3.x, pr_vec3.y, pr_vec3.z);
}
