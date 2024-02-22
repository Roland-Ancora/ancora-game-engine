#include "../include/ageUI.h"



using namespace age;



void UIObject::set_parent_object(UIObject* obj)
{
	obj->childs.push_back(this);
	parent_obj = obj;
	height = real_height / parent_obj->get_aspects_ratio();
}

void UIObject::set_position_x(float x)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x - pos_x, 0.0f, 0.0f));
	pos_x = x;
}

void UIObject::set_position_y(float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, y * parent_obj->aspects_ratio - pos_y * parent_obj->aspects_ratio, 0.0f));
	pos_y = y;
}

void UIObject::set_position(float x, float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x - pos_x, y * parent_obj->aspects_ratio - pos_y * parent_obj->aspects_ratio, 0.0f));
	pos_x = x;
	pos_y = y;
}

void UIObject::set_width(float w)
{
	float new_scale = w / width;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	width = w;
	real_height = w * aspects_ratio;
	height = real_height / parent_obj->get_aspects_ratio();
}

void UIObject::set_height(float h)
{
	float new_scale = h / height;
	height = h;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	real_height = height * parent_obj->get_aspects_ratio();
	width = real_height / aspects_ratio;
}




ShaderProgram UIWindow::main_shader_prog_ui;
UIWindow* UIWindow::active_ui_window = 0;

UIWindow::UIWindow(Camera3D* cam)
{
	main_shader_prog_ui = ShaderProgram::create_shader_program("resources/shaders/age_main_3d_ui_shader.vert", "resources/shaders/age_main_3d_ui_shader.frag");
	camera = cam;
	real_width = camera->get_ortho_fov() * camera->get_aspects_ratio();
	real_height = camera->get_ortho_fov();
	aspects_ratio = float(real_height) / real_width;
	active_ui_window = this;
}

void UIWindow::show_and_update()
{
	active_ui_window = this;
	global_model_mat = glm::translate(glm::mat4(1), camera->get_position() + camera->get_view_point_in_sphere() + camera->get_view_point_in_sphere());
	global_model_mat = glm::rotate(global_model_mat, camera->get_rotation().x, glm::vec3(0, 1, 0));
	global_model_mat = glm::rotate(global_model_mat, -camera->get_rotation().y, glm::vec3(1, 0, 0));
	global_model_mat = glm::translate(global_model_mat, glm::vec3(-real_width / 2, -real_height / 2, 0.0f));
	global_model_mat = glm::scale(global_model_mat, glm::vec3(real_width, real_width, 1.0f));
	Camera::get_active_camera()->use_shader(&UIWindow::main_shader_prog_ui);
	for (int i = 0; i < childs.size(); i++)
		childs[i]->show_and_update(global_model_mat, glm::mat4(1));
	Camera::get_active_camera()->use_main_shader();
}




void UIImage::show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui)
{
	if (is_active) {
		model_mat_in_ui = m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		global_model_mat = m_mat * (translate_mat * rotate_mat * scale_mat);
		Camera::get_active_camera()->set_model_matrix(&global_model_mat);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);

		glBindVertexArray(vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindTexture(GL_TEXTURE_2D, *texture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vaoID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(global_model_mat, model_mat_in_ui);
	}
}

void UIImage::set_texture(Texture2D* tex)
{
	texture = tex;
	aspects_ratio = float(tex->get_height()) / tex->get_width();
	real_height = width * aspects_ratio;
	ver_pos_data[7] = aspects_ratio;
	ver_pos_data[10] = aspects_ratio;
	height = real_height / parent_obj->get_aspects_ratio();
}

void UIImage::set_z_val(float z)
{
	z_var = z;
	ver_pos_data[2] = z;
	ver_pos_data[5] = z;
	ver_pos_data[8] = z;
	ver_pos_data[11] = z;
}

void UIImage::set_part_from_x_begin(float pc)
{
	shown_part_from_x_l = pc;
	ver_pos_data[3] = shown_part_from_x_l;
	ver_pos_data[6] = shown_part_from_x_l;
	tex_pos_data[2] = 1.0f - shown_part_from_x_l;
	tex_pos_data[4] = 1.0f - shown_part_from_x_l;
}




// In tex[4] first element - default texture, second - focus texture,
// third - pressed texture, fourth - disable texture.
UIButton::UIButton(UIObject* p_obj, Texture2D* tex[4])
{
	UIImage(); 
	set_parent_object(p_obj);
	set_textures(tex);
}

// In tex[4] first element - default texture, second - focus texture,
// third - pressed texture, fourth - disable texture.
inline void UIButton::set_textures(Texture2D* tex[4])
{
	set_default_texture(tex[0]);
	focus_texture = tex[1];
	pressed_texture = tex[2];
	disable_texture = tex[3];
}

void UIButton::show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui)
{
	if (is_active) {
		UIImage::show_and_update(m_mat, m_mat_ui);

		if (is_btn_active) {
			double mouse_pos_x, mouse_pos_y;
			glfwGetCursorPos(*Window::get_active_window(), &mouse_pos_x, &mouse_pos_y);
			mouse_pos_x /= Window::get_active_window()->get_width();
			mouse_pos_y /= Window::get_active_window()->get_height();
			mouse_pos_y /= Camera::get_active_camera()->get_aspects_ratio();

			glm::vec3 scale_history, traslation_history;
			glm::vec3 self_model_mat__skew_vec;
			glm::quat self_model_mat__orient_history;
			glm::vec4 self_model_mat__persp_history;
			glm::decompose(model_mat_in_ui, scale_history, self_model_mat__orient_history, traslation_history, self_model_mat__skew_vec, self_model_mat__persp_history);
			if (mouse_pos_x < traslation_history.x + scale_history.x
				&& mouse_pos_x > traslation_history.x
				&& mouse_pos_y > traslation_history.y
				&& mouse_pos_y < traslation_history.y + scale_history.y * aspects_ratio) {
				
				texture = focus_texture;
				if (is_btn_pressed)
					is_btn_pressed = false;
				if (InputEventsControler::is_mouse_button_pressed_and_released(GLFW_MOUSE_BUTTON_LEFT))
					is_btn_pressed = true;
			}
			else {
				texture = default_texture;
				is_btn_pressed = false;
			}
		}
		else
			is_btn_pressed = false;
	}
}




void UIText::show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui)
{
	if (is_active) {
		Camera::get_active_camera()->use_shader(Font::get_shader_program());
		GLuint shader_z_var_loc = glGetUniformLocation(Camera::get_active_camera()->get_active_shader()->get_shader_program_id(), "z_val");
		glUniform1f(shader_z_var_loc, z_var);

		model_mat_in_ui = m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		global_model_mat = m_mat * (translate_mat * rotate_mat * scale_mat);
		Camera::get_active_camera()->set_model_matrix(&global_model_mat);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int line_num = 1;
		float next_sym_pos = 0.0f;
		for (int i = 0; i < str.size(); i++) {
			if (str_eds_symbol_nums.size() > line_num - 1 && i > str_eds_symbol_nums[line_num - 1]) {
				line_num++;
				next_sym_pos = 0.0f;
			}
			else if (str[i] == 10) {
				line_num++;
				next_sym_pos = 0.0f;
				continue;
			}

			float text_data[] = {
			(next_sym_pos + font->Characters[str[i]].Bearing.x) * font_t_size, (font_t_size + distance_between_lines) * line_num - font->Characters[str[i]].Bearing.y * font_t_size, 0.0f, 0.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x + font->Characters[str[i]].Size.x) * font_t_size, (font_t_size + distance_between_lines) * line_num - font->Characters[str[i]].Bearing.y * font_t_size, 1.0f, 0.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x + font->Characters[str[i]].Size.x) * font_t_size, (font_t_size + distance_between_lines) * line_num + (-font->Characters[str[i]].Bearing.y + font->Characters[str[i]].Size.y) * font_t_size, 1.0f, 1.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x) * font_t_size, (font_t_size + distance_between_lines) * line_num + (-font->Characters[str[i]].Bearing.y + font->Characters[str[i]].Size.y) * font_t_size, 0.0f, 1.0f
			};
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), text_data, GL_STATIC_DRAW);

			glBindVertexArray(vaoID);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

			glBindTexture(GL_TEXTURE_2D, font->Characters[str[i]].TextureID);
			glBindVertexArray(vaoID);
			glDrawArrays(GL_QUADS, 0, 4);

			next_sym_pos += font->Characters[str[i]].Advance;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		UIWindow::get_active_ui_window()->use_main_ui_shader();
	}
}

void UIText::calculate_str_end_symbol_nums()
{
	str_eds_symbol_nums.clear();
	float line_size_counter = 0.0f;
	for (int i = 0; i < str.size(); i++) {
		if (line_size_counter + font->Characters[str[i]].Advance * font_t_size > 1.0f || str[i] == 10) {
			str_eds_symbol_nums.push_back(i);
			line_size_counter = 0.0f;
			continue;
		}
		line_size_counter += font->Characters[str[i]].Advance * font_t_size;
	}
}

void UIText::calculate_str_end_symbol_nums_w_words()
{
	str_eds_symbol_nums.clear();
	float line_size_counter = 0.0f;
	bool is_first_word_in_line = true;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == 10) {
			str_eds_symbol_nums.push_back(i);
			line_size_counter = 0.0f;
			continue;
		}
		else if (str[i] == 32) {
			line_size_counter += font->Characters[str[i]].Advance * font_t_size;
			if (line_size_counter > 1.0f) {
				str_eds_symbol_nums.push_back(i);
				line_size_counter = 0.0f;
			}
			continue;
		}
		if (is_next_word_bigger_then_width(i, line_size_counter)) {
			str_eds_symbol_nums.push_back(i - 1);
			line_size_counter = 0.0f;
		}
		while (str[i] != 10 && str[i] != 32 && str[i] != 0) {
			line_size_counter += font->Characters[str[i]].Advance * font_t_size;
			i++;
		}
	}
}

bool UIText::is_next_word_bigger_then_width(int char_start_word, float now_str_width)
{
	while (str[char_start_word] != 32 && str[char_start_word] != 10 && str[char_start_word] != 0) {
		now_str_width += font->Characters[str[char_start_word]].Advance * font_t_size;
		char_start_word++;
	}
	if (now_str_width > 1.0f)
		return true;
	return false;
}

void UIText::set_z_val(float z)
{
	z_var = z;
}

void UIText::set_string(std::string string)
{
	str = string;
	calculate_str_end_symbol_nums_w_words();
}




void UIOrganizedContainer::set_size(float w, float h)
{
	float new_scale = w / width;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	width = w;
	height = h;
	aspects_ratio = height / width * parent_obj->get_aspects_ratio();
}

void UIOrganizedContainer::update_childs()
{
	if (childs.size() > 0) {
		for (int i = 0; i < childs.size(); i++) {
			childs[i]->set_width(1.0f);
			if (i == 0) {
				childs[i]->set_position_y(0.0f);
				continue;
			}
			childs[i]->set_position_y(childs[i - 1]->get_position_y() + childs[i - 1]->get_height());
		}
		max_height = (childs[childs.size() - 1]->get_position_y() + childs[childs.size() - 1]->get_height());
	}
}

void UIOrganizedContainer::show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui)
{
	if (is_active) {
		if (InputEventsControler::get_scroll_y() < 0.0f && now_list_pos + height < max_height) {
			float new_list_pos = now_list_pos - InputEventsControler::get_scroll_y() / 100;
			if (new_list_pos + height > max_height)
				new_list_pos = max_height - height;
			list_translate_mat = glm::translate(list_translate_mat, glm::vec3(0.0f, (now_list_pos - new_list_pos) * UIWindow::get_active_ui_window()->get_aspects_ratio(), 0.0f));
			now_list_pos = new_list_pos;
		}
		else if (InputEventsControler::get_scroll_y() > 0.0f && now_list_pos > 0.0f) {
			float new_list_pos = now_list_pos - InputEventsControler::get_scroll_y() / 100;
			if (new_list_pos < 0)
				new_list_pos = 0;
			list_translate_mat = glm::translate(list_translate_mat, glm::vec3(0.0f, (now_list_pos - new_list_pos) * UIWindow::get_active_ui_window()->get_aspects_ratio(), 0.0f));
			now_list_pos = new_list_pos;
		}
		
		global_model_mat = UIWindow::get_active_ui_window()->get_ui_window_matrix() * list_translate_mat * m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		model_mat_in_ui = list_translate_mat * m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(global_model_mat, model_mat_in_ui);
	}
}




void UIContainer::show_and_update(glm::mat4 m_mat, glm::mat4 m_mat_ui)
{
	if (is_active) {
		model_mat_in_ui = m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		global_model_mat = m_mat * (translate_mat * rotate_mat * scale_mat);

		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(global_model_mat, model_mat_in_ui);
	}
}

// IT'S NAY BE WRONG REALIZATION. MUST BE TESTED
void UIContainer::set_size(float w, float h)
{
	float new_scale_w = w / width;
	float new_scale_h = h / height;
	height = h;
	width = w;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale_w, new_scale_w, 1.0f));
	real_height = height * parent_obj->get_aspects_ratio();
	aspects_ratio = real_height / width;
}
