//#################################################################//
//
//								UIElements3D
//	
//		The UIElements3D module is responsible for showing UI 
//	elements like images, texts, buttons and other in 3D space.
// 
//		This module has external dependencies: OpenGL, GLM.
//		This module has dependencies: ageTexture2D, ageCamera,
//	ageWindow, ageInputEventsControler, ageShaderProgram and basic
//	AGE types.
//
//
//#################################################################//



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ageTexture2D.h"
#include "ageWindow.h"
#include "ageCamera.h"
#include "ageInputEventsControler.h"
#include "ageShaderProgram.h"
#include "basic_types/age_rotate_vector.h"



namespace age {

	class UIElement3D {
	protected:
		bool is_active = true;
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		glm::mat4 independ_rotate_mat = glm::mat4(1);
		glm::mat4 independ_translate_mat = glm::mat4(1);
		float width = 1.0f, height = 1.0f;
		float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
		float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;
		float shown_part_from_x_begin = 1.0f;
		glm::mat4 finally_mat = glm::mat4(1);
		bool camera_follow = false;
		static ShaderProgram ui3d_sh_prog;
		static GLuint ui3d_sh_prog__alpha_channel_uniform;
		UIElement3D* parent_element = 0;
	public:
		static void Init();
		virtual void show() {};
	public:
		void disable() { is_active = false; }
		void enable() { is_active = true; }
		void enable_camera_follow() { camera_follow = true; }
		void disable_camera_follow() { camera_follow = false; }
		void set_parent_element(UIElement3D* elem) { parent_element = elem; }
		glm::mat4 _get_model_matrix() { return finally_mat; }
	};

	class UIImage3D : public UIElement3D {
	protected:
		Texture2D* img_texture;
		GLuint vboID;
		GLuint vaoID;
		bool middle_rotate_point_active = false;
		glm::vec3 rotate_arm = glm::vec3(0.0f, 0.0f, 1.0f);
		float alpha_channel = 1.0f;
		float vert_tex_pos_data[16] = {
			-(width * shown_part_from_x_begin / 2), height / 2,
			0.0f, 0.0f,
			width* shown_part_from_x_begin / 2, height / 2,
			1.0f + (1.0f - shown_part_from_x_begin), 0.0f,
			width* shown_part_from_x_begin / 2, -(height / 2),
			1.0f + (1.0f - shown_part_from_x_begin), 1.0f,
			-(width * shown_part_from_x_begin / 2), -(height / 2),
			0.0f, 1.0f
		};
		float texture_pos_x = 0.0f, texture_pos_y = 0.0f;
	public:
		UIImage3D() { glGenBuffers(1, &vboID); glGenVertexArrays(1, &vaoID); }
		virtual void show();
	public:
		glm::vec3 get_position() { return glm::vec3(x_pos, y_pos, z_pos); }
		void set_alpha_channel(float alpha) { alpha_channel = alpha;  }
		float get_alpha_channel() { return alpha_channel; }
		void set_independ_rotation(float angle, rotate_vector vec) { independ_rotate_mat = glm::rotate(glm::mat4(1), angle, vec); }
		void set_independ_translation(glm::vec3 pos) { independ_translate_mat = glm::translate(glm::mat4(1), pos); }
	public:
		void set_texture(Texture2D* texture);
		void set_size(float w, float h);
		void set_size(float w);
		void set_position(float x, float y, float z);
		void move(float x, float y, float z);
		void set_rotation(float angle, rotate_vector vec);
		void set_rotation(glm::mat4 rotation_matrix);
		void rotate(float angle, rotate_vector vec);
		void rotate(glm::mat4 rotation_matrix);
		void set_part_from_x_begin(float pc);
		void use_middle_rotate_point(glm::vec3 arm = glm::vec3(0.0f, 0.0f, 1.0f));
		void disable_middle_rotate_point();
		void set_arm(glm::vec3 arm);
		void set_texture_position(float x, float y);
	};

	class UIButton3D : public UIImage3D {
		Texture2D* default_texture, * focus_texture, * pressed_texture, * disable_texture;
		bool is_btn_active = true;
		bool is_btn_pressed = false, is_btn_pressed_next_frame = false;
	public:
		UIButton3D() { UIImage3D(); }
		UIButton3D(Texture2D* tex, Texture2D* focus_tex, Texture2D* pressed_tex, Texture2D* dis_tex);
		virtual void show();
	public:
		void set_default_texture(Texture2D* tex) { default_texture = tex; set_texture(default_texture); }
		void set_focus_texture(Texture2D* tex) { focus_texture = tex; }
		void set_pressed_texture(Texture2D* tex) { pressed_texture = tex; }
		void set_disable_texture(Texture2D* tex) { disable_texture = tex; }
		void set_textures(Texture2D* def, Texture2D* foc, Texture2D* prs, Texture2D* dis);
		bool is_pressed() { return is_btn_pressed; }
		void disable_button() { is_btn_active = false; img_texture = disable_texture; }
		void enable_button() { is_btn_active = true; img_texture = default_texture; }
	};

}
