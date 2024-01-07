//#####################################################################//
//
//								Camera
//	
//		The Camera classes is responsible for the main shader program,
//	the transformation of the camera in space, clearning buffers
//	and the display location in the window.
//
//		The Camera class has external dependencies: GLM, GLFW, OpenGL.
//		The Camera class has dependencies: ageShaderProgram.
// 
// 
// 
//		The Camera class has main shader program (main_shader_prog).
//	This is the shader program, that is default active and activing by
//	the use_main_shader function. The now_active_shader points to the
//	active shader program. This is necessary to change model matrix
//	in the shader when change drawing object. The model_matrix is a
//	pointer and he just points to a model matrix of rendering object.
// 
//		What is the static field active_camera? First of all, that's
//	require for a Model3D objects, for a send model matrix to the
//	camera (because Camera controls shaders program). Also, in future
//	this mechanism allows create control over a few camers.
//
//
//#####################################################################//



#pragma once
#include "../depends/glm/glm.hpp"
#include "../depends/glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../include/ageShaderProgram.h"



namespace age {

	typedef char camera_type;
	const camera_type AGE_CAMERA_PERSPECTIVE = 0;
	const camera_type AGE_CAMERA_ORTHOGRAPHIC = 1;



	class Camera {
	protected:
		Camera();
		static Camera* active_camera;
		camera_type camera_mode = AGE_CAMERA_ORTHOGRAPHIC;
		ShaderProgram main_shader_prog;
		ShaderProgram* now_active_shader = &main_shader_prog;
		glm::mat4 projection_matrix, view_matrix, MVP_matrix, MV_matrix;
		glm::mat4* model_matrix = &ident_mat;
		float aspects_ratio = 1.0f; // it's width to height ratio
		static glm::mat4 ident_mat;
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	public:
		static Camera* get_active_camera() { return active_camera; }
		camera_type get_camera_type() { return camera_mode; }
		float get_aspects_ratio() { return aspects_ratio; }
		glm::vec3 get_position() { return position; }
		glm::mat4* get_proj_matrix() { return &projection_matrix; }
		glm::mat4* get_model_view_matrix() { return &MV_matrix; }
		glm::mat4* get_view_matrix() { return &view_matrix; }
		ShaderProgram* get_active_shader() { return now_active_shader; }
		virtual void clear_buffers() {};
		virtual void set_aspects_ratio(float x, float y) {};
		void set_model_matrix(glm::mat4* model_mat);
		void use_shader(ShaderProgram* shader_prog);
		void use_main_shader();
	};



	class Camera2D : public Camera {
		float fov = 15.0f, z_near = 0.01f, z_far = 1.0f;
		glm::vec3 vector_up = glm::vec3(0.0f, 1.0f, 0.0f);
		float rotate_angle = glm::radians(90.0f); // by radians
		static Camera2D* active_2d_camera;
		void calculate_and_use_MVP_matrices();
	public:
		Camera2D();
		static Camera2D* get_active_2d_camera() { return active_2d_camera; }
		float get_fov() { return fov; }
		virtual void clear_buffers();
		virtual void set_aspects_ratio(float x, float y);
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
		glm::vec3 rotation;
		float persp_fov = 55.0f, persp_z_near = 0.01f, persp_z_far = 50.0f;
		float ortho_fov = 15.0f, ortho_z_near = 0.01f, ortho_z_far = 50.0f;
		void calculate_and_use_MVP_matrices();
		void calculate_rotation_from_radians();

		// test
		static Camera3D* active_3d_camera;
	public:
		Camera3D();
		glm::vec2 get_rotation() { return rotation_radians_xy; }
		float get_ortho_fov() { return ortho_fov; }
		float get_ortho_z_near() { return ortho_z_near; }
		float get_ortho_z_far() { return ortho_z_far; }
		virtual void clear_buffers();
		virtual void set_aspects_ratio(float x, float y);
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

		//test
		static Camera3D* get_active_3d_camera() { return active_3d_camera; }
		glm::vec3 get_rot_lookat() { return rotation; }
	};


}
