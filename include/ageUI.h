#pragma once
#include "ageWindow.h"
#include "ageCamera.h"
#include "ageTexture2D.h"
#include "ageShaderProgram.h"
#include "ageInputEventsControler.h"
#include "ageFont.h"
#include <vector>
#include "../depends/glm/gtx/matrix_decompose.hpp"



namespace age {

	enum UIElement_aspects { WIDTH_ASPECT, HEIGHT_ASPECT };




	class UIWindow;

	class UIObject {
	protected:
		bool is_active = true;
		float origin_width = 0.1f, origin_height = 0.1f;
		float origin_pos_x, origin_pos_y;
		float z_var = -1.0f;
		float aspects_ratio = 1.0f; // It's always height to width ratio
		int ui_lvl = 0;
		glm::mat4 model_mat = glm::mat4(1);
		glm::mat4 translate_mat = glm::mat4(1), scale_mat = glm::mat4(1), rotate_mat = glm::mat4(1);
		UIObject* parent_obj = 0;
		std::vector<UIObject*> childs;
		UIElement_aspects last_specified_ratio = WIDTH_ASPECT;
		glm::vec3 self_model_mat__scale_history = glm::vec3(1.0f, 1.0f, 1.0f), self_model_mat__trans_history = glm::vec3(0.0f, 0.0f, 1.0f);
		bool is_redimensions_beforea_update = false;

		friend UIWindow;
		static Texture2D default_unvis_texture;
		int get_ui_lvl() { return ui_lvl; }
	public:
		UIObject() { scale_mat = glm::scale(scale_mat, glm::vec3(origin_width, origin_width, 1.0f)); }
		void set_parent_object(UIObject* obj);
		glm::mat4 get_model_matrix() { return model_mat; }
		virtual void show_and_update() {}
		void set_position_x(float x);
		void set_position_y(float y);
		void set_position(float x, float y);
		void set_width(float w);
		void set_height(float h);
		virtual void set_size(float w, float h);
		virtual void set_width_only(float w); // must realize
		virtual void set_height_only(float h); // must realize
		void set_z_val(float z) { z_var = z; }
		float get_position_x() { return origin_pos_x; }
		float get_position_y() { return origin_pos_y; }
		float get_width() { return origin_width; }
		float get_height() { return origin_height; }
		float get_real_position_x() { return self_model_mat__trans_history.x; }
		float get_real_position_y() { return self_model_mat__trans_history.y; }
		float get_real_width() { return self_model_mat__scale_history.x; }
		float get_real_height() { return self_model_mat__scale_history.y; }
		float get_aspects_ratio() { return aspects_ratio; }
		void activate() { is_active = true; }
		void disable() { is_active = false; }
		UIObject* get_parent_obj() { return parent_obj; }
	};

	class UIWindow : public UIObject {
		static ShaderProgram main_shader_prog_ui;
		static UIWindow* active_ui_window;
		Camera3D* camera;
		float real_width, real_height;
		glm::mat4 real_model_mat = glm::mat4(1);
	public:
		UIWindow(Camera3D* cam);
		virtual void show_and_update();
		static UIWindow* get_active_ui_window() { return active_ui_window; }
		glm::mat4 get_real_model_mat() { return real_model_mat; }
		float get_real_width() { return real_width; }
		float get_real_height() { return real_height; }
		void use_main_ui_shader() { Camera::get_active_camera()->use_shader(&UIWindow::main_shader_prog_ui); }
	};

	class UIImage : public UIObject {
	protected:
		Texture2D* texture = &default_unvis_texture;
		float shown_part_from_x_begin = 1.0f;
	public:
		virtual void show_and_update();
		UIImage() { UIObject(); }
		UIImage(UIObject* p_obj) { UIObject(); set_parent_object(p_obj); }
		UIImage(UIObject* p_obj, Texture2D* texture) { UIObject(); set_parent_object(p_obj); set_texture(texture); }
		void set_texture(Texture2D* tex);
		void set_part_from_x_begin(float pc) { shown_part_from_x_begin = pc; };
	};

	class UIText : public UIObject {
		std::string str = "";
		float font_t_size = 0.1f, distance_between_lines = font_t_size / 2;
		std::vector<int> str_eds_symbol_nums;
		Font* font;
		void calculate_str_end_symbol_nums();
		void calculate_str_end_symbol_nums_w_words();
		bool is_next_word_bigger_then_width(int char_start_word, float now_str_width);
	public:
		UIText() { }
		virtual void show_and_update();
		void set_font(Font* f) { font = f; }
		void set_font_size(float size) { font_t_size = size; distance_between_lines = font_t_size / 2; calculate_str_end_symbol_nums_w_words(); }
		void set_string(std::string string);
	};

	class UIButton : public UIImage {
		Texture2D *default_texture, *focus_texture, *pressed_texture, *disable_texture;
		bool is_btn_active = true;
		bool is_btn_pressed = false, is_btn_pressed_next_frame = false;
		float real_pos_x_start_for_mouse, real_pos_y_star_for_mouse;
		float real_pos_x_finish_for_mouse, real_pos_y_finish_for_mouse;

		virtual void show_and_update();
		virtual void set_texture(Texture2D* tex) { UIImage::set_texture(tex); }
	public:
		UIButton() { UIImage(); }
		UIButton(UIObject* p_obj) { UIImage(); set_parent_object(p_obj); }
		UIButton(UIObject* p_obj, Texture2D* tex, Texture2D* focus_tex, Texture2D* pressed_tex, Texture2D* dis_tex)
		{
			UIImage(); set_parent_object(p_obj); set_default_texture(tex); focus_texture = focus_tex;  pressed_texture = pressed_tex;  disable_texture = dis_tex;
		}
		void set_default_texture(Texture2D* tex) { default_texture = tex; set_texture(default_texture); }
		void set_focus_texture(Texture2D* tex) { focus_texture = tex; }
		void set_pressed_texture(Texture2D* tex) { pressed_texture = tex; }
		void set_disable_texture(Texture2D* tex) { disable_texture = tex; }
		void set_textures(Texture2D* def, Texture2D* foc, Texture2D* prs, Texture2D* dis) { default_texture = def; set_texture(default_texture); focus_texture = foc; pressed_texture = prs; disable_texture = dis; }
		bool is_pressed() { return is_btn_pressed; }
		void disable() { is_btn_active = false; texture = disable_texture; }
		void enable() { is_btn_active = true; texture = default_texture; }
	};

	class UIOrganizedContainer : public UIObject {
		float max_height = 0.0f, now_list_pos = 0.0f;
		glm::mat4 list_translate_mat = glm::mat4(1);
		float aspect_real_ratio = 1.0f;
	public:
		virtual void set_size(float w, float h);
		void update_childs();
		virtual void show_and_update();
	};

}
