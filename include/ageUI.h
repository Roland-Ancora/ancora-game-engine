//#################################################################//
//
//									UI
//	
//		The UI module is responsible for showing UI elements on 
//	display. Implemented as a separate framebuffer, which, after
//	rendering, is superimposed on the main framebuffer. All objects
//	are UIObject and can contain dependent children. The root object
//	is a singltone UIWindow object.
// 
//		This module has external dependencies: OpenGL, GLM, STD,
//	FFmpeg.
//		This module has dependencies: ageTexture2D, ageCamera,
//	ageWindow, ageInputEventsControler, ageFont, ageShaderProgram
//	and ageProgrsmClock.
//
//
//#################################################################//



#pragma once
#include <cinttypes>
#include <iostream>
#include <vector>
#include <glm/gtx/matrix_decompose.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include "ageTexture2D.h"
#include "ageCamera.h"
#include "ageWindow.h"
#include "ageInputEventsControler.h"
#include "ageFont.h"
#include "ageCONSTANTS.h"
#include "ageProgramClock.h"



namespace age {
	
	class UIObject {
	protected:
		bool is_active = true;
		float width = 0.1f, height = 0.1f;
		float real_height = 0.1f;
		float pos_x = 0.0f, pos_y = 0.0f;
		float z_var = 0.1f;
		float aspects_ratio = 1.0f;
		float alpha = 1.0f;
		glm::mat4 translate_mat = glm::mat4(1), scale_mat = glm::mat4(1), rotate_mat = glm::mat4(1);
		glm::mat4 model_mat = glm::mat4(1);
		UIObject* parent_obj = 0;
		bool focused = false;
		std::vector<UIObject*> childs;
		std::vector<UIObject*> childs_front;
		std::vector<UIObject*> childs_back;
		static UIObject* focused_object;
	protected:
		void is_focused_calculate(glm::mat4 p_ui_mat);
	public:
		~UIObject();
		UIObject();
		static UIObject* get_focused_object() { return focused_object; }
		static void _clear_focused_object() { focused_object = 0; }
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat) {}
		virtual void set_z_val(float z);
		virtual void disable();
	public:
		float get_aspects_ratio() { return aspects_ratio; }
		bool get_active_state() { return is_active; }
		UIObject* get_parent_object() { return parent_obj; }
		glm::mat4 get_model_matrix() { return model_mat; }
		float get_position_x() { return pos_x; }
		float get_position_y() { return pos_y; }
		float get_width() { return width; }
		float get_height() { return height; }
		float get_z_val() { return z_var; }
		float get_alpha() { return alpha; }
		int get_childs_count() { return static_cast<int>(childs.size()); }
		UIObject* get_child(int id) { return childs[id]; }
		bool is_focused() { return focused; }
		void set_alpha(float a) { alpha = a; }
	public:
		void set_parent_object(UIObject* obj);
		void set_position_x(float x);
		void set_position_y(float y);
		void set_position(float x, float y);
		void set_width(float w);
		void set_height(float h);
		void _add_child_object(UIObject* obj);
		void enable();
		bool unbind_child(UIObject* obj);
		void _replace_backfront_child(UIObject* obj);
	};

	class UIWindow : public UIObject {
		static UIWindow* active_ui_window;
		ShaderProgram main_shader_prog_ui;
		ShaderProgram main_shader_fin_prog_ui;
		bool flag_redraw = false;
		glm::mat4 proj_mat, view_mat, VP_mat;
		GLuint FBO, FBO_texture, RBO_depth_trapharet;
		GLuint vboIDs[2];
		GLuint vaoID;
		float ver_pos_data[8] = {
			-1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f - 2 * aspects_ratio,
			-1.0f, 1.0f - 2 * aspects_ratio,
		};
		float tex_pos_data[8] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};
		GLuint alpha_uniform_loc;
	public:
		UIWindow(std::uint16_t resolutin_w, std::uint16_t resolution_h);
		static UIWindow* get_active_ui_window() { return active_ui_window; }
	public:
		void use_main_ui_shader() { Camera::get_active_camera()->use_shader(&main_shader_prog_ui); }
		GLuint _get_alpha_uniform_location() { return alpha_uniform_loc; }
	public:
		void show();
	};

	class UIImage : public UIObject {
	protected:
		Texture2D* texture = 0;
		float shown_part_from_x_l = 1.0f;
		float shown_part_from_y_l = 1.0f;
		float shown_part_from_y_e = 1.0f;
		GLuint vboIDs[2];
		GLuint vaoID;
		float ver_pos_data[12] = {
			z_var, 0.0f, 0.0f,
			z_var, 0.0f, 1.0f,
			z_var, 1.0f, 1.0f,
			z_var, 1.0f, 0.0f
		};
		float tex_pos_data[8] = {
			0.0f,										1.0f - shown_part_from_y_e,
			1.0f + (1.0f - shown_part_from_x_l),		1.0f - shown_part_from_y_e,
			1.0f + (1.0f - shown_part_from_x_l),		1.0f + (1.0f - shown_part_from_y_l),
			0.0f,										1.0f + (1.0f - shown_part_from_y_l)
		};
	public:
		UIImage();
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
		virtual void set_z_val(float z);
	public:
		float get_part_from_x_begin() { return shown_part_from_x_l; }
	public:
		void set_texture(Texture2D* tex);
		void set_part_from_x_begin(float pc);
		void set_part_from_y_begin(float pc);
		void set_part_from_y_end(float pc);
	}; 

	class UIButton : public UIImage {
		Texture2D* default_texture, * focus_texture, * pressed_texture, * disable_texture;
		bool is_btn_active = true;
		bool is_btn_pressed = false;
		bool is_btn_focus = false;
	public:
		UIButton() { UIImage(); }
		UIButton(UIObject* p_obj) { UIImage(); set_parent_object(p_obj); }
		UIButton(UIObject* p_obj, Texture2D* tex[4]);
		virtual void disable() { is_active = false; is_btn_pressed = false; }
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
	public:
		void set_default_texture(Texture2D* tex) { default_texture = tex; set_texture(default_texture); }
		void set_focus_texture(Texture2D* tex) { focus_texture = tex; }
		void set_pressed_texture(Texture2D* tex) { pressed_texture = tex; }
		void set_disable_texture(Texture2D* tex) { disable_texture = tex; }
		bool is_pressed() { return is_btn_pressed; }
		void disable_button() { is_btn_active = false; texture = disable_texture; }
		void enable_button() { is_btn_active = true; texture = default_texture; }
		inline void set_textures(Texture2D* tex[4]);
	};

	class UIText : public UIObject {
		std::string str = "";
		float font_t_size = 0.1f, distance_between_lines = 0.0f;
		float max_width = 1.0f;
		std::vector<int> str_eds_symbol_nums;
		Font* font;
		GLuint vboIDs[2];
		GLuint vaoID;
	private:
		void calculate_str_end_symbol_nums();
		void calculate_str_end_symbol_nums_w_words();
		bool is_next_word_bigger_then_width(int char_start_word, float now_str_width);
	public:
		UIText() { glGenBuffers(2, vboIDs); glGenVertexArrays(1, &vaoID); }
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
		virtual void set_z_val(float z);
		virtual void set_size(float w, float h) {} // DOESN'T MAKE SENSE
	public:
		void set_font(Font* f) { font = f; }
		void set_font_size(float size) { font_t_size = size; distance_between_lines = font_t_size / 2; calculate_str_end_symbol_nums_w_words(); }
		int get_lines_count() { return static_cast<int>(str_eds_symbol_nums.size()); }
	public:
		void set_string(std::string string);
	};

	class UIOrganizedContainer : public UIObject {
		float max_height = 0.0f, now_list_pos = 0.0f;
		glm::mat4 list_translate_mat = glm::mat4(1);
		float aspect_real_ratio = 1.0f;
	public:
		virtual void set_size(float w, float h);
		virtual void set_z_val(float z) {} // MUST BE REALIZED
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
	public:
		void update_childs();
	};

	class UIContainer : public UIObject {
	public:
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
		virtual void set_z_val(float z) {} // MUST BE REALIZED
		virtual void set_size(float w, float h);
	};

	class UISlider : public UIImage {
		UIButton s_btn;
		float border_x1 = 0.0f, border_x2 = 1.0f;
		bool btn_hold = false;
		float cursor_last_pos_x = 0.0f;
	public:
		UISlider() { s_btn.set_parent_object(this); }
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
	public:
		void set_slider_button_texture_normal(Texture2D* tex) { s_btn.set_default_texture(tex); }
		void set_slider_button_texture_focus(Texture2D* tex) { s_btn.set_focus_texture(tex); }
		void set_slider_button_texture_press(Texture2D* tex) { s_btn.set_pressed_texture(tex); }
		void set_slider_button_borders(float x1, float x2) { border_x1 = x1, border_x2 = x2; }
		void set_slider_value(float val) { s_btn.set_position_x((100.0f - (border_x1 + border_x2)) * val + border_x1); }
		float get_slider_value() { return (s_btn.get_position_x() - border_x1) / (100.0f - border_x1 - border_x2); }
	};

	class UIVideo : public UIObject {
		Texture2D video_texture;
		AVFormatContext* format_context;
		AVCodecParameters* codec_parameters[2] { 0, 0};
		AVCodec* codec[2];
		AVCodecContext* codec_context[2];
		AVPacket* packet;
		AVFrame* frame[2];
		std::vector<AVFrame*> video_frames_stream;
		AVFrame* frameRGB;
		uint8_t* rgb_buffer = 0;
		int rgb_buffer_linsizes = 0;
		int rgb_buffer_num_bytes = 0;
		SwsContext* sws_cntx;
		const int8_t video_stream_id = 0, sound_stream_id = 1;
		int8_t video_stream_id_in_context = -1, sound_stream_id_in_context = -1;
		double ms_passed_after_last_frame = 0.0;
		float video_duration_now = 0.0f;
		GLuint vboIDs[2];
		GLuint vaoID;
		float ver_pos_data[12] = {
			z_var, 0.0f, 0.0f,
			z_var, 0.0f, 1.0f,
			z_var, 1.0f, 1.0f,
			z_var, 1.0f, 0.0f
		};
		float tex_pos_data[8] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		double _time_base_ffmpeg = 0.0;
		float _frame_rate_sec = 0.0f;
	private:
		void set_first_frame();
	public:
		~UIVideo();
		UIVideo();
		virtual void show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat);
	public:
		int get_duration() { return static_cast<int>(format_context->duration) < 0 ? -1 : static_cast<int>(format_context->duration) / AV_TIME_BASE; }
	public:
		int load_video_file(const char* file_name);
		void restart_video();
		void play();
		void pause();
	};

}
