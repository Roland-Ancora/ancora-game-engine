#include "ageCamera.h"



using namespace age;



_Framebuffer::_Framebuffer(uint16_t resolution_w, uint16_t resolution_h)
{
	tex_w = resolution_w;
	tex_h = resolution_h;

	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

	glGenTextures(1, &fbo_texture_id);
	glBindTexture(GL_TEXTURE_2D, fbo_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution_w, resolution_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture_id, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo_dt);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_dt);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution_w, resolution_h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_dt);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("AGE::ERROR::_FRAMEBUFER::FRAMEBUFFER:: Framebuffer is not complete!\n");

	glClearColor(1.8f, 0.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





glm::mat4 Camera::ident_mat = glm::mat4(1);
Camera* Camera::active_camera = 0;

Camera::Camera(camera_hdr use_hdr)
{
	if (active_camera != 0)
		exit(AGE_CAMERA_OBJECT_OVERLAPPING_ERROR);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	active_camera = this;
	is_hdr = use_hdr;

	glGenBuffers(2, vboIDs);
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vbo_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_buf);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), ver_pos_data_fbufs, GL_STATIC_DRAW);
	shader_for_framebuffer_texture_draw = ShaderProgram::create_shader_program(AGE_DEFAULT_UI_FINNALY_TEXTURE_VERTEX_SHADER, AGE_DEFAULT_UI_FINNALY_TEXTURE_FRAGMENT_SHADER);

	shader_for_finaly_framebuffer_rendering = ShaderProgram::create_shader_program(AGE_FINALY_FRAMEBUFFER_TEXTURE_VERTEX_SHADER, AGE_FINALY_FRAMEBUFFER_TEXTURE_FRAGMENT_SHADER);
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

void Camera::set_shader_subroutine(int shader_type, const char* name)
{
	GLuint s = glGetSubroutineIndex(now_active_shader->get_shader_program_id(), shader_type, name);
	glUniformSubroutinesuiv(shader_type, 1, &s);
}

uint8_t Camera::add_framebuffer(uint16_t resolution_w, uint16_t resolution_h)
{
	_Framebuffer* bufs_recov = new _Framebuffer[usr_bufers_cnt];
	for (uint8_t i = 0; i < usr_bufers_cnt; i++)
		bufs_recov[i] = usr_framebuffers[i];
	usr_bufers_cnt++;
	delete[] usr_framebuffers;
	usr_framebuffers = new _Framebuffer[usr_bufers_cnt];
	for (uint8_t i = 0; i < usr_bufers_cnt - 1; i++)
		usr_framebuffers[i] = bufs_recov[i];
	delete[] bufs_recov;
	usr_framebuffers[usr_bufers_cnt - 1] = _Framebuffer(resolution_w, resolution_h);
	return usr_bufers_cnt - 1;
}

void Camera::use_framebuffer(uint8_t id)
{
	glBindFramebuffer(GL_FRAMEBUFFER, usr_framebuffers[id].get_fbo_id());
	glViewport(0, 0, usr_framebuffers[id].get_width(), usr_framebuffers[id].get_height());
	glClearColor(1.0f, 0.0f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Camera::show_framebuffers()
{
	for (int i = 0; i < usr_bufers_cnt; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Camera::get_active_camera()->use_shader(&shader_for_framebuffer_texture_draw);

		glBindVertexArray(vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_buf);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, usr_framebuffers[i].get_texture_id());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vaoID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		Camera::get_active_camera()->use_main_shader();
	}
}

void Camera::_create_main_default_renderbuffer()
{
	glGenFramebuffers(1, &finaly_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, finaly_framebuffer);
	main_framebuffer = finaly_framebuffer;

	glGenTextures(1, &finaly_framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, finaly_framebuffer_texture);
	if (is_hdr)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, defoult_window_resolution_w, defoult_window_resolution_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, defoult_window_resolution_w, defoult_window_resolution_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finaly_framebuffer_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &finaly_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, finaly_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, defoult_window_resolution_w, defoult_window_resolution_h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, finaly_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("AGE::ERROR::_FRAMEBUFER::FRAMEBUFFER:: Main framebuffer is not complete!\n");

	glClearColor(1.8f, 0.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::show_finaly_framebuffer()
{
	use_shader(&shader_for_finaly_framebuffer_rendering);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, finaly_framebuffer_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vaoID);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	use_main_shader();
}





Camera2D* Camera2D::active_2d_camera = 0;

Camera2D::Camera2D(camera_hdr use_hdr)
{
	active_2d_camera = this;
	is_hdr = use_hdr;
	main_shader_prog = ShaderProgram::create_shader_program(AGE_DEFAULT_2D_VERTEX_SHADER, AGE_DEFAULT_2D_FRAGMENT_SHADER);
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
	Camera::set_aspects_ratio(x, y);
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

Camera3D::Camera3D(camera_hdr use_hdr)
{
	active_3d_camera = this;
	is_hdr = use_hdr;
	glEnable(GL_DEPTH_TEST);
	main_shader_prog = ShaderProgram::create_shader_program(AGE_DEFAULT_3D_VERTEX_SHADER, AGE_DEFAULT_3D_FRAGMENT_SHADER);
	GLuint default_color_processing_subroutine = glGetSubroutineIndex(main_shader_prog.get_shader_program_id(), GL_FRAGMENT_SHADER, "default_color_only");
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &default_color_processing_subroutine);
	camera_mode = AGE_CAMERA_PERSPECTIVE;
	projection_matrix = glm::perspective(glm::radians(persp_fov), aspects_ratio, persp_z_near, persp_z_far);
	glUseProgram(main_shader_prog.get_shader_program_id());
	calculate_rotation_from_radians();
	calculate_and_use_MVP_matrices();
}

void Camera3D::calculate_and_use_MVP_matrices()
{
	view_matrix = glm::lookAt(position, position + view_point_in_sphere, view_up_point);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, main_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera3D::set_aspects_ratio(float x, float y)
{
	Camera::set_aspects_ratio(x, y);
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

void Camera3D::set_view_up_point(float x, float y, float z)
{
	view_up_point.x = x;
	view_up_point.y = y;
	view_up_point.z = z;
	calculate_and_use_MVP_matrices();
}

void Camera3D::set_view_point_in_sphere(float x, float y, float z)
{
	view_point_in_sphere.x = x;
	view_point_in_sphere.y = y;
	view_point_in_sphere.z = z;
	calculate_and_use_MVP_matrices();
}
