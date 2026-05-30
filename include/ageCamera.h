//#################################################################//
//
//								Camera
//	
//		The Camera classes is responsible for the main shader 
//	program, the transformation of the camera in space, clearning 
//	buffers and the display location in the window.
//		The camera_type type - type of camera: perspective or
//	orthographic.
//
//		The Camera class has external dependencies: GLM, GLFW,
//	OpenGL.
//		The Camera class has dependencies: ShaderProgram.
// 
//		The Camera class has main shader program (main_shader_prog).
//	This is the shader program, that is default active and activing
//	by	the use_main_shader function. The now_active_shader points
//	to the	active shader program. This is necessary to change model
//	matrix	in the shader when change drawing object. The
//	model_matrix is a pointer and he just points to a model matrix 
//	of rendering object.
// 
//		Camera, Camera2D and Camera3D are singltone classes. What 
//	is the static field active_camera? First of all, that's require
//	for a Model3D objects, for a send model matrix to the
//	camera (because Camera controls shaders program). Also, in 
//	future this mechanism allows create control over a few camers.
//
//
//#################################################################//



#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ageShaderProgram.h"
#include "ageCONSTANTS.h"



namespace age {

	typedef char camera_type;
	const camera_type AGE_CAMERA_PERSPECTIVE = 0;
	const camera_type AGE_CAMERA_ORTHOGRAPHIC = 1;
	typedef bool camera_hdr;
	const camera_hdr AGE_CAMERA_HDR_ON = true;
	const camera_hdr AGE_CAMERA_HDR_OFF = false;



	class _Framebuffer {
		GLuint fbo_id;
		GLuint fbo_texture_id;
		GLuint rbo_dt;
		uint16_t tex_w, tex_h;
	public:
		_Framebuffer() {}
		_Framebuffer(uint16_t resolution_w, uint16_t resolution_h);
		GLuint get_fbo_id() { return fbo_id; }
		GLuint get_texture_id() { return fbo_texture_id; }
		uint16_t get_width() { return tex_w; }
		uint16_t get_height() { return tex_h; }
	};



	class Camera {
	protected:
		Camera(camera_hdr use_hdr = AGE_CAMERA_HDR_OFF);
	protected:
		static Camera* active_camera;
		static glm::mat4 ident_mat;
		camera_type camera_mode = AGE_CAMERA_ORTHOGRAPHIC;
		ShaderProgram main_shader_prog;
		ShaderProgram* now_active_shader = &main_shader_prog;
		glm::mat4 projection_matrix, view_matrix, MVP_matrix;
		glm::mat4* model_matrix = &ident_mat;
		float aspects_ratio = 1.0f; // it's width to height ratio
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

		GLuint finaly_framebuffer_texture, finaly_framebuffer;
		ShaderProgram shader_for_finaly_framebuffer_rendering;
		GLuint finaly_rbo;
		GLuint main_framebuffer = 0;
		camera_hdr is_hdr = AGE_CAMERA_HDR_OFF;

		uint16_t defoult_window_resolution_w, defoult_window_resolution_h;
		ShaderProgram shader_for_framebuffer_texture_draw;
		uint8_t usr_bufers_cnt = 0;
		_Framebuffer* usr_framebuffers;
		GLuint vboIDs[2];
		GLuint vbo_buf;
		GLuint vaoID;
		float ver_pos_data[8] = {
			-1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f - 2 * aspects_ratio,
			-1.0f, 1.0f - 2 * aspects_ratio,
		};
		float ver_pos_data_fbufs[8] = {
			-1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f
		};
		float tex_pos_data[8] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};
	public:
		static Camera* get_active_camera() { return active_camera; }
		virtual void clear_buffers() {};
		virtual void set_aspects_ratio(float x, float y) { aspects_ratio = x / y; };
	public:
		camera_type get_camera_type() { return camera_mode; }
		float get_aspects_ratio() { return aspects_ratio; }
		glm::vec3 get_position() { return position; }
		ShaderProgram* get_active_shader() { return now_active_shader; }
		void set_clear_color(float r, float g, float b) { glClearColor(r, g, b, 1.0f); }
		void set_main_shader(ShaderProgram shader_prog) { main_shader_prog = shader_prog; use_main_shader(); }
		void use_main_framebufer() { glBindFramebuffer(GL_FRAMEBUFFER, main_framebuffer); glViewport(0, 0, defoult_window_resolution_w, defoult_window_resolution_h); }
	public:
		void set_model_matrix(glm::mat4* model_mat);
		void use_shader(ShaderProgram* shader_prog);
		void use_main_shader();
		void set_shader_subroutine(int shader_type, const char* name);
		uint8_t add_framebuffer(uint16_t resolution_w, uint16_t resolution_h);
		void use_framebuffer(uint8_t id);
		void show_framebuffers();
		void show_finaly_framebuffer();
	public:
		void _set_window_resolution(uint16_t w, uint16_t h) { defoult_window_resolution_w = w; defoult_window_resolution_h = h; }
		void _create_main_default_renderbuffer(); // used in Window after _set_window_resolution
	};



	class Camera2D : public Camera {
		float fov = 15.0f, z_near = 0.01f, z_far = 1.0f;
		glm::vec3 vector_up = glm::vec3(0.0f, 1.0f, 0.0f);
		float rotate_angle = glm::radians(90.0f); // by radians
		static Camera2D* active_2d_camera;
	private:
		void calculate_and_use_MVP_matrices();
	public:
		Camera2D(camera_hdr use_hdr = AGE_CAMERA_HDR_OFF);
		static Camera2D* get_active_2d_camera() { return active_2d_camera; }
		virtual void clear_buffers();
		virtual void set_aspects_ratio(float x, float y);
	public:
		float get_fov() { return fov; }
	public:
		void set_fov(float new_fov);
		void move(float x, float y);
		void move_x(float x);
		void move_y(float y);
		void set_position(float x, float y);
		void set_position_x(float x);
		void set_position_y(float y);
	};



	class Camera3D : public Camera {
		glm::vec2 rotation_radians_xy = glm::vec2(3.14159f, 0.0f);
		float persp_fov = 55.0f, persp_z_near = 0.01f, persp_z_far = 50.0f;
		float ortho_fov = 15.0f, ortho_z_near = 0.01f, ortho_z_far = 50.0f;
		glm::vec3 view_up_point = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 view_point_in_sphere = glm::vec3(0.0f, 0.0f, 1.0f);
		static Camera3D* active_3d_camera;
	private:
		void calculate_and_use_MVP_matrices();
		void calculate_rotation_from_radians();
	public:
		Camera3D(camera_hdr use_hdr = AGE_CAMERA_HDR_OFF);
		static Camera3D* get_active_3d_camera() { return active_3d_camera; }
		virtual void clear_buffers();
		virtual void set_aspects_ratio(float x, float y);
	public:
		glm::vec2 get_rotation() { return rotation_radians_xy; }
		glm::vec3 get_view_point_in_sphere() { return view_point_in_sphere; }
		float get_ortho_fov() { return ortho_fov; }
		float get_ortho_z_near() { return ortho_z_near; }
		float get_ortho_z_far() { return ortho_z_far; }
		float get_persp_fov() { return persp_fov; }
		float get_persp_z_near() { return persp_z_near; }
		float get_persp_z_far() { return persp_z_far; }
		glm::vec3 get_view_up_point() { return view_up_point; }
	public:
		void set_view_up_point(float x, float y, float z);
		void set_position(float x, float y, float z);
		void move(float x, float y, float z);
		void set_rotation(float x, float y);
		void rotate(float x, float y);
		void set_orthographic();
		void set_orthographic(float fov);
		void set_orthographic(float z_near, float z_far);
		void set_perspective();
		void set_perspective(float fov);
		void set_perspective(float z_near, float z_far);
		glm::vec3 get_pixel_position_in_world_coords(int pixel_pos_x, int pixel_pos_y, int window_height);
		void set_view_point_in_sphere(float x, float y, float z);
	};

}
