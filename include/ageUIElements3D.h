//########################################################################//
//
//								UIElements3D
//	
//		The UIElements3D module is responsible for showing UI elements like
//	images, texts, buttons and other in 3D space.
// 
//		This module has external dependencies: OpenGL, GLM.
//		This module has dependencies: ageTexture2D, ageCamera.
//
//
//########################################################################//



#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../depends/glm/glm.hpp"
#include "../depends/glm/gtc/matrix_transform.hpp"
#include "ageTexture2D.h"
#include "ageWindow.h"
#include "ageCamera.h"
#include "ageInputEventsControler.h"
#include "basic_types/age_rotate_vector.h"
#include "additions/ageInputEventsFunctions.h"



namespace age {

	class UIElement3D {
	protected:
		bool is_active = true;
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		float width = 1.0f, height = 1.0f;
		float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
		float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;
		float shown_part_from_x_begin = 1.0f;
		glm::mat4 finally_mat = glm::mat4(1);
		bool camera_follow = false;
	public:
		virtual void show() {};
		void disable() { is_active = false; }
		void enable() { is_active = true; }
		void enable_camera_follow() { camera_follow = true; }
		void disable_camera_follow() { camera_follow = false; }
	};

	class UIImage3D : public UIElement3D {
	protected:
		Texture2D* img_texture;
		GLuint vboIDs[3];
		GLuint vaoID;
	public:
		UIImage3D() { glGenBuffers(3, vboIDs); glGenVertexArrays(1, &vaoID); }
		virtual void show();
		void set_texture(Texture2D* texture);
		void set_size(float w, float h);
		void set_size(float w);
		void set_position(float x, float y, float z);
		void set_rotation(float angle, rotate_vector vec);
		void rotate(float angle, rotate_vector vec);
		glm::vec3 get_position() { return glm::vec3(x_pos, y_pos, z_pos); }
		void set_part_from_x_begin(float pc) { shown_part_from_x_begin = pc; };
	};

	class UIButton3D : public UIImage3D {
		Texture2D* default_texture, * focus_texture, * pressed_texture, * disable_texture;
		bool is_btn_active = true;
		bool is_btn_pressed = false, is_btn_pressed_next_frame = false;
	public:
		UIButton3D() { UIImage3D(); }
		UIButton3D(Texture2D* tex, Texture2D* focus_tex, Texture2D* pressed_tex, Texture2D* dis_tex)
		{
			UIImage3D(); set_default_texture(tex); focus_texture = focus_tex;  pressed_texture = pressed_tex;  disable_texture = dis_tex;
		}
		void set_default_texture(Texture2D* tex) { default_texture = tex; set_texture(default_texture); }
		void set_focus_texture(Texture2D* tex) { focus_texture = tex; }
		void set_pressed_texture(Texture2D* tex) { pressed_texture = tex; }
		void set_disable_texture(Texture2D* tex) { disable_texture = tex; }
		void set_textures(Texture2D* def, Texture2D* foc, Texture2D* prs, Texture2D* dis) { default_texture = def; set_texture(default_texture); focus_texture = foc; pressed_texture = prs; disable_texture = dis; }
		bool is_pressed() { return is_btn_pressed; }
		void disable_button() { is_btn_active = false; img_texture = disable_texture; }
		void enable_button() { is_btn_active = true; img_texture = default_texture; }
		virtual void show();
	};

}
