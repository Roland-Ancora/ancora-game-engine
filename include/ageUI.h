#pragma once
#include <../depends/glm/gtx/matrix_decompose.hpp>
#include <vector>

#include "ageTexture2D.h"
#include "ageShaderProgram.h"
#include "ageCamera.h"
#include "ageWindow.h"
#include "ageInputEventsControler.h"
#include "ageFont.h"



namespace age {

	class UIWindow;

	class UIObject {
	protected:
		bool is_active = true;
		float width = 1.0f, height = 1.0f;
		float real_height = 1.0f;
		float pos_x = 0.0f, pos_y = 0.0f;
		float z_var = -1.0f;
		float aspects_ratio = 1.0f; // it's height to width ratio (width / height) in global coordinate system
		glm::mat4 global_model_mat = glm::mat4(1); // model matrix with UIWindow matrix
		glm::mat4 model_mat_in_ui = glm::mat4(1); // model matrix without UIWindow matrix, it's need for UIButton and mouse position
		glm::mat4 translate_mat = glm::mat4(1), scale_mat = glm::mat4(1), rotate_mat = glm::mat4(1);
		UIObject* parent_obj = 0;
		std::vector<UIObject*> childs;
	public:
		UIObject() {}
		~UIObject() { if (parent_obj != 0) parent_obj->unbind_child(this); }
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui) {}
		void set_parent_object(UIObject* obj);
		void set_position_x(float x);
		void set_position_y(float y);
		void set_position(float x, float y);
		void set_width(float w);
		void set_height(float h);
		virtual void set_z_val(float z) { z_var = z; }
		virtual void set_size(float w, float h) {} // vitrual in UI1  // NOT REALIZED
		float get_position_x() { return pos_x; }
		float get_position_y() { return pos_y; }
		float get_width() { return width; }
		float get_height() { return height; }
		void enable() { is_active = true; }
		virtual void disable() { is_active = false; }
		UIObject* get_parent_obj() { return parent_obj; }
		float get_aspects_ratio() { return aspects_ratio; }
		bool unbind_child(UIObject* obj);
		int get_childs_count() { return childs.size(); }
		bool get_active_state() { return is_active; }
	};

	class UIWindow : public UIObject {
		static ShaderProgram main_shader_prog_ui;
		static UIWindow* active_ui_window;
		Camera3D* camera; // can't set Camera field because show_and_update method use Camera3D methods.
		float real_width, real_height; // width and height in global coordinate system
	public:
		UIWindow(Camera3D* cam);
		void show_and_update();
		float get_aspects_ratio() { return aspects_ratio; }
		static UIWindow* get_active_ui_window() { return active_ui_window; }
		glm::mat4 get_ui_window_matrix() { return global_model_mat; }
		void use_main_ui_shader() { Camera::get_active_camera()->use_shader(&UIWindow::main_shader_prog_ui); }
	};

	class UIImage : public UIObject {
	protected:
		Texture2D* texture = 0;
		float shown_part_from_x_l = 1.0f;
		GLuint vboIDs[2];
		GLuint vaoID;
		float ver_pos_data[12] = {
			0.0f, 0.0f, z_var,
			shown_part_from_x_l, 0.0f, z_var,
			shown_part_from_x_l, aspects_ratio, z_var,
			0.0f, aspects_ratio, z_var
		};
		float tex_pos_data[8] = {
			0.0f, 0.0f,
			1.0f + (1.0f - shown_part_from_x_l), 0.0f,
			1.0f + (1.0f - shown_part_from_x_l), 1.0f,
			0.0f, 1.0f
		};
	public:
		UIImage() { UIObject(); glGenBuffers(2, vboIDs); glGenVertexArrays(1, &vaoID); }
		UIImage(UIObject* p_obj) { UIImage(); set_parent_object(p_obj); }
		UIImage(UIObject* p_obj, Texture2D* texture) { UIImage(); set_parent_object(p_obj); set_texture(texture); }
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui);
		void set_texture(Texture2D* tex);
		virtual void set_z_val(float z);
		virtual void set_size(float w, float h) {} // MUST BE REALIZED
		void set_part_from_x_begin(float pc);
		float get_part_from_x_begin() { return shown_part_from_x_l; }
		float get_z_val() { return z_var; }
	};

	class UIButton : public UIImage {
		Texture2D* default_texture, * focus_texture, * pressed_texture, * disable_texture;
		bool is_btn_active = true;
		bool is_btn_pressed = false;
	public:
		UIButton() { UIImage(); }
		UIButton(UIObject* p_obj) { UIImage(); set_parent_object(p_obj); }
		UIButton(UIObject* p_obj, Texture2D* tex[4]);
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui);
		void set_default_texture(Texture2D* tex) { default_texture = tex; set_texture(default_texture); }
		void set_focus_texture(Texture2D* tex) { focus_texture = tex; }
		void set_pressed_texture(Texture2D* tex) { pressed_texture = tex; }
		void set_disable_texture(Texture2D* tex) { disable_texture = tex; }
		inline void set_textures(Texture2D* tex[4]);
		bool is_pressed() { return is_btn_pressed; }
		void disable_button() { is_btn_active = false; texture = disable_texture; }
		void enable_button() { is_btn_active = true; texture = default_texture; }
		virtual void disable() { is_active = false; is_btn_pressed = false; }
	};

	class UIText : public UIObject {
		std::string str = "";
		float font_t_size = 0.1f, distance_between_lines = 0.0f;
		float max_width = 1.0f;
		std::vector<int> str_eds_symbol_nums;
		Font* font;
		GLuint vboIDs[2];
		GLuint vaoID;
		void calculate_str_end_symbol_nums();
		void calculate_str_end_symbol_nums_w_words();
		bool is_next_word_bigger_then_width(int char_start_word, float now_str_width);
	public:
		UIText() { glGenBuffers(2, vboIDs); glGenVertexArrays(1, &vaoID); }
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui);
		virtual void set_z_val(float z); // MUST BE REALIZED
		virtual void set_size(float w, float h) {} // DOESN'T MAKE SENSE
		void set_font(Font* f) { font = f; }
		void set_font_size(float size) { font_t_size = size; distance_between_lines = font_t_size / 2; calculate_str_end_symbol_nums_w_words(); }
		void set_string(std::string string);
		int get_lines_count() { return str_eds_symbol_nums.size(); }
	};

	class UIOrganizedContainer : public UIObject {
		float max_height = 0.0f, now_list_pos = 0.0f;
		glm::mat4 list_translate_mat = glm::mat4(1);
		float aspect_real_ratio = 1.0f;
	public:
		virtual void set_size(float w, float h);
		virtual void set_z_val(float z) {} // MUST BE REALIZED
		void update_childs();
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui);
	};

	class UIContainer : public UIObject {
	public:
		virtual void show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui);
		virtual void set_z_val(float z) {} // MUST BE REALIZED
		virtual void set_size(float w, float h);
	};

}
