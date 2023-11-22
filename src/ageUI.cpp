#define NEW_UI_V3

#include "../include/ageUI.h"



using namespace age;




Texture2D UIObject::default_unvis_texture;

void UIObject::set_parent_object(UIObject* obj)
{
	if (last_specified_ratio == HEIGHT_ASPECT) {
		float new_scale = Camera::get_active_camera()->get_aspects_ratio() / obj->aspects_ratio;
		scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	}
	obj->childs.push_back(this);
	ui_lvl = obj->get_ui_lvl() + 1;
	parent_obj = obj;
}

void UIObject::set_position_x(float x)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x - origin_pos_x, 0.0f, 0.0f));
	origin_pos_x = x;
	is_redimensions_beforea_update = true;
}

void UIObject::set_position_y(float y)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_position_y called before seting parent object\n");
		exit(51);
	}
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, y * parent_obj->aspects_ratio - origin_pos_y * parent_obj->aspects_ratio, 0.0f));
	origin_pos_y = y;
	is_redimensions_beforea_update = true;
}

void UIObject::set_position(float x, float y)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_position called before seting parent object\n");
		exit(51);
	}
	translate_mat = glm::translate(translate_mat, glm::vec3(x - origin_pos_x, y * parent_obj->aspects_ratio - origin_pos_y * parent_obj->aspects_ratio, 0.0f));
	origin_pos_x = x;
	origin_pos_y = y;
	is_redimensions_beforea_update = true;
}

void UIObject::set_width(float w)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_width called before seting parent object\n");
		exit(51);
	}
	float new_scale = w / origin_width;
	if (last_specified_ratio != WIDTH_ASPECT && ui_lvl > 1)
		new_scale = (w / parent_obj->aspects_ratio) / origin_width;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	origin_width = w;
	origin_height = w * aspects_ratio / parent_obj->aspects_ratio;
	last_specified_ratio = WIDTH_ASPECT;
	is_redimensions_beforea_update = true;
}

void UIObject::set_height(float h)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_hieght called before seting parent object\n");
		exit(51);
	}
	float new_scale = h / origin_height;
	if (last_specified_ratio != HEIGHT_ASPECT && ui_lvl <= 1)
		new_scale = h / origin_height;
	else if (last_specified_ratio != HEIGHT_ASPECT && ui_lvl > 1)
		new_scale = (h * parent_obj->aspects_ratio) / origin_height;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, new_scale, 1.0f));
	origin_height = h;
	origin_width = parent_obj->aspects_ratio / aspects_ratio * h;
	last_specified_ratio = HEIGHT_ASPECT;
	is_redimensions_beforea_update = true;
}

void UIObject::set_size(float w, float h)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_size called before seting parent object\n");
		exit(51);
	}
	float new_scale = w / origin_width;
	if (last_specified_ratio != WIDTH_ASPECT && ui_lvl <= 1)
		new_scale = w / origin_width * Camera::get_active_camera()->get_aspects_ratio();
	else if (last_specified_ratio != WIDTH_ASPECT && ui_lvl > 1)
		new_scale = (w / parent_obj->aspects_ratio) / origin_width;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, 1.0f, 1.0f));
	origin_width = w;

	new_scale = h / origin_height;
	if (last_specified_ratio != HEIGHT_ASPECT && ui_lvl <= 1)
		new_scale = h / origin_height / Camera::get_active_camera()->get_aspects_ratio();
	else if (last_specified_ratio != HEIGHT_ASPECT && ui_lvl > 1)
		new_scale = (h * parent_obj->aspects_ratio) / origin_height;
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, new_scale, 1.0f));
	origin_height = h;
	aspects_ratio = h / w;
	is_redimensions_beforea_update = true;
}

void UIObject::set_width_only(float w)
{

}

void UIObject::set_height_only(float h)
{

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
	model_mat = glm::mat4(1);
}

void UIWindow::show_and_update()
{
	active_ui_window = this;
	real_model_mat = glm::translate(glm::mat4(1), camera->get_position() + camera->get_rot_lookat() + camera->get_rot_lookat());
	real_model_mat = glm::rotate(real_model_mat, camera->get_rotation().x, glm::vec3(0, 1, 0));
	real_model_mat = glm::rotate(real_model_mat, -camera->get_rotation().y, glm::vec3(1, 0, 0));
	real_model_mat = glm::translate(real_model_mat, glm::vec3(-real_width / 2, -real_height / 2, 0.0f));
	real_model_mat = glm::scale(real_model_mat, glm::vec3(real_width, real_width, 1.0f));
	Camera::get_active_camera()->use_shader(&UIWindow::main_shader_prog_ui);
	for (int i = 0; i < childs.size(); i++)
		childs[i]->show_and_update();
	Camera::get_active_camera()->use_main_shader();
}



void UIImage::show_and_update()
{
	if (is_active) {
		model_mat = parent_obj->get_model_matrix() * (translate_mat * rotate_mat * scale_mat);
		glm::mat4 real_mat = UIWindow::get_active_ui_window()->get_real_model_mat() * parent_obj->get_model_matrix() * (translate_mat * rotate_mat * scale_mat);
		Camera::get_active_camera()->set_model_matrix(&real_mat);

		float ver_pos_data[] = {
			0.0f, 0.0f, z_var,
			1.0f * shown_part_from_x_begin, 0.0f, z_var,
			1.0f * shown_part_from_x_begin, aspects_ratio, z_var,
			0.0f, aspects_ratio, z_var
		};
		float tex_pos_data[] = {
			0.0f, 0.0f,
			1.0f + (1.0f - shown_part_from_x_begin), 0.0f,
			1.0f + (1.0f - shown_part_from_x_begin), 1.0f,
			0.0f, 1.0f
		};

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

		glm::vec3 self_model_mat__skew_vec;
		glm::quat self_model_mat__orient_history;
		glm::vec4 self_model_mat__persp_history;
		glm::decompose(model_mat, self_model_mat__scale_history, self_model_mat__orient_history, self_model_mat__trans_history, self_model_mat__skew_vec, self_model_mat__persp_history);

		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update();
	}
}

void UIImage::set_texture(Texture2D* tex)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_texture called before seting parent object\n");
		exit(51);
	}
	texture = tex;
	float prev_asp = aspects_ratio;
	aspects_ratio = float(tex->get_height()) / tex->get_width();
	if (last_specified_ratio == WIDTH_ASPECT)
		origin_height = origin_width * aspects_ratio / parent_obj->get_aspects_ratio();
	else {
		float scale_val = prev_asp / aspects_ratio;
		scale_mat = glm::scale(scale_mat, glm::vec3(scale_val, scale_val, 1.0f));
		origin_width = parent_obj->get_aspects_ratio() / aspects_ratio * origin_height;
	}
}



void UIText::show_and_update()
{
	if (is_active) {
		Camera::get_active_camera()->use_shader(&font->text_sh_program);

		model_mat = parent_obj->get_model_matrix() * (translate_mat * rotate_mat * scale_mat);
		glm::mat4 real_mat = UIWindow::get_active_ui_window()->get_real_model_mat() * parent_obj->get_model_matrix() * (translate_mat * rotate_mat * scale_mat);

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
			Camera::get_active_camera()->set_model_matrix(&real_mat);

			
			float text_data[] = {
			(next_sym_pos + font->Characters[str[i]].Bearing.x)* font_t_size, (font_t_size + distance_between_lines)* line_num - font->Characters[str[i]].Bearing.y * font_t_size, 0.0f, 0.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x + font->Characters[str[i]].Size.x)* font_t_size, (font_t_size + distance_between_lines)* line_num - font->Characters[str[i]].Bearing.y * font_t_size, 1.0f, 0.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x + font->Characters[str[i]].Size.x)* font_t_size, (font_t_size + distance_between_lines)* line_num + (-font->Characters[str[i]].Bearing.y + font->Characters[str[i]].Size.y) * font_t_size, 1.0f, 1.0f,
			(next_sym_pos + font->Characters[str[i]].Bearing.x)* font_t_size, (font_t_size + distance_between_lines)* line_num + (-font->Characters[str[i]].Bearing.y + font->Characters[str[i]].Size.y) * font_t_size, 0.0f, 1.0f
			};
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), text_data, GL_STATIC_DRAW);

			glBindVertexArray(vaoID);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

			glBindTexture(GL_TEXTURE_2D, font->Characters[str[i]].TextureID);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindVertexArray(vaoID);
			glDrawArrays(GL_QUADS, 0, 4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glDisableVertexAttribArray(0);
			glDisable(GL_BLEND);
			glBindTexture(GL_TEXTURE_2D, 0);

			next_sym_pos += font->Characters[str[i]].Advance;
		}

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

void UIText::set_string(std::string string)
{
	str = string;
	calculate_str_end_symbol_nums_w_words();
}



void UIButton::show_and_update()
{
	if (is_active) {
		UIImage::show_and_update();

		if (is_btn_active) {
			double mouse_pos_x, mouse_pos_y;
			glfwGetCursorPos(*Window::get_active_window(), &mouse_pos_x, &mouse_pos_y);
			mouse_pos_x /= Window::get_active_window()->get_width();
			mouse_pos_y /= Window::get_active_window()->get_height();
			mouse_pos_y /= Camera::get_active_camera()->get_aspects_ratio();

			if (mouse_pos_x < self_model_mat__trans_history.x + self_model_mat__scale_history.x
				&& mouse_pos_x > self_model_mat__trans_history.x
				&& mouse_pos_y > self_model_mat__trans_history.y
				&& mouse_pos_y < self_model_mat__trans_history.y + self_model_mat__scale_history.y * aspects_ratio) {
				texture = focus_texture;

				if (is_btn_pressed)
					is_btn_pressed = false;
				if (InputEventsControler::is_mouse_button_pressed_and_released(*Window::get_active_window(), GLFW_MOUSE_BUTTON_LEFT))
					is_btn_pressed = true;
			}
			else
				texture = default_texture;
			if (is_redimensions_beforea_update) {
				is_redimensions_beforea_update = false;
				if (is_btn_pressed)
					is_btn_pressed = false;
			}
		}
		else
			is_btn_pressed = false;
	}
}



void UIOrganizedContainer::set_size(float w, float h)
{
	if (parent_obj == 0) {
		printf("ERROR::UI: set_size called before seting parent object\n");
		exit(51);
	}
	float new_scale = w / origin_width;
	if (last_specified_ratio != WIDTH_ASPECT && ui_lvl <= 1)
		new_scale = w / origin_width * Camera::get_active_camera()->get_aspects_ratio();
	else if (last_specified_ratio != WIDTH_ASPECT && ui_lvl > 1)
		new_scale = (w / parent_obj->get_aspects_ratio()) / origin_width;
	scale_mat = glm::scale(scale_mat, glm::vec3(new_scale, 1.0f, 1.0f));
	origin_width = w;

	origin_height = h;
	aspects_ratio = 1.0f;
	aspect_real_ratio = (h / w) * parent_obj->get_aspects_ratio();
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
		max_height = (childs[childs.size() - 1]->get_position_y() / aspect_real_ratio + childs[childs.size() - 1]->get_height() / aspect_real_ratio);
	}
}

void UIOrganizedContainer::show_and_update()
{
	if (is_active) {
		if (InputEventsControler::get_scroll_y() < 0.0f && now_list_pos + origin_height / Camera::get_active_camera()->get_aspects_ratio() < max_height / Camera::get_active_camera()->get_aspects_ratio()) {
			float new_list_pos = now_list_pos - InputEventsControler::get_scroll_y() / 100;
			if (new_list_pos + origin_height / Camera::get_active_camera()->get_aspects_ratio() > max_height / Camera::get_active_camera()->get_aspects_ratio())
				new_list_pos = max_height / Camera::get_active_camera()->get_aspects_ratio() - origin_height / Camera::get_active_camera()->get_aspects_ratio();
			list_translate_mat = glm::translate(list_translate_mat, glm::vec3(0.0f, now_list_pos - new_list_pos, 0.0f));
			now_list_pos = new_list_pos;
		}
		else if (InputEventsControler::get_scroll_y() > 0.0f && now_list_pos > 0.0f) {
			float new_list_pos = now_list_pos - InputEventsControler::get_scroll_y() / 100;
			if (new_list_pos < 0)
				new_list_pos = 0;
			list_translate_mat = glm::translate(list_translate_mat, glm::vec3(0.0f, now_list_pos - new_list_pos, 0.0f));
			now_list_pos = new_list_pos;
		}
		model_mat = list_translate_mat * parent_obj->get_model_matrix() * ((translate_mat)*rotate_mat * scale_mat);
		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update();
	}
}
