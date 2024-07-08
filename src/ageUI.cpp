#include "../include/ageUI.h"



using namespace age;



UIObject::~UIObject()
{
	if (parent_obj != 0)
		parent_obj->unbind_child(this);
	else
		UIWindow::get_active_ui_window()->unbind_child(this);
}

UIObject::UIObject()
{
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, height, width));
	model_mat = scale_mat;
}

void UIObject::set_parent_object(UIObject* obj)
{
	if (parent_obj != 0)
		parent_obj->unbind_child(this);
	else
		UIWindow::get_active_ui_window()->unbind_child(this);
	parent_obj = obj; 
	obj->_add_child_object(this);
}

void UIObject::set_position_x(float x)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, 0.0f, x - pos_x));
	model_mat = translate_mat * scale_mat * rotate_mat;
	pos_x = x;
}

void UIObject::set_position_y(float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, (y - pos_y) * (parent_obj == 0 ? UIWindow::get_active_ui_window()->get_aspects_ratio() : parent_obj->get_aspects_ratio()), 0.0f));
	model_mat = translate_mat * scale_mat * rotate_mat;
	pos_y = y;
}

void UIObject::set_position(float x, float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, (y - pos_y) * (parent_obj == 0 ? UIWindow::get_active_ui_window()->get_aspects_ratio() : parent_obj->get_aspects_ratio()), x - pos_x));
	model_mat = translate_mat * scale_mat * rotate_mat;
	pos_x = x;
	pos_y = y;
}

void UIObject::set_width(float w)
{
	float new_scale = w / width;
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, new_scale, new_scale));
	model_mat = translate_mat * scale_mat * rotate_mat;
	width = w;
	real_height = w * aspects_ratio;
	height = real_height / parent_obj->get_aspects_ratio();
}

void UIObject::set_height(float h)
{
	float new_scale = h / height;
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, new_scale, new_scale));
	model_mat = translate_mat * scale_mat * rotate_mat;
	height = h;
	real_height = height * parent_obj->get_aspects_ratio();
	width = real_height / aspects_ratio;
}

void UIObject::set_z_val(float z)
{
	z_var = z;
	if (parent_obj != 0)
		parent_obj->_replace_backfront_child(this);
}

void UIObject::_add_child_object(UIObject* obj)
{
	if (std::count(childs.begin(), childs.end(), obj) > 0)
		return;
	childs.push_back(obj);
	obj->get_z_val() >= z_var ? childs_back.push_back(obj) : childs_front.push_back(obj);
}

void UIObject::enable()
{
	is_active = true;
}

void UIObject::disable()
{
	is_active = false;
}

bool UIObject::unbind_child(UIObject* obj)
{
	auto result{ std::find(begin(childs), end(childs), obj) };
	if (result == end(childs))
		return false;
	childs.erase(result);
	auto result_back { std::find(begin(childs_back), end(childs_back), obj) };
	if (result_back != end(childs_back)) {
		childs_back.erase(result_back);
		return true;
	}
	auto result_front{ std::find(begin(childs_front), end(childs_front), obj) };
	if (result_front != end(childs_front)) {
		childs_front.erase(result_front);
		return true;
	}
	return false;
}

void UIObject::_replace_backfront_child(UIObject* obj)
{
	auto result_back{ std::find(begin(childs_back), end(childs_back), obj) };
	auto result_front{ std::find(begin(childs_front), end(childs_front), obj) };
	if (obj->get_z_val() < z_var) {
		if (result_front != end(childs_front))
			return;
		else {
			childs_back.erase(result_back);
			childs_front.push_back(obj);
		}
	}
	else if (obj->get_z_val() >= z_var) {
		if (result_back != end(childs_back))
			return;
		else {
			childs_front.erase(result_front);
			childs_back.push_back(obj);
		}
	}
}





UIWindow* UIWindow::active_ui_window = 0;

UIWindow::UIWindow(std::uint16_t resolution_w, std::uint16_t resolution_h)
{
	active_ui_window = this;
	main_shader_prog_ui = ShaderProgram::create_shader_program(AGE_DEFAULT_UI_VERTEX_SHADER, AGE_DEFAULT_UI_FRAGMENT_SHADER);
	main_shader_fin_prog_ui = ShaderProgram::create_shader_program("resources/shaders/age_main_ui_fin_texture_shader.vert", "resources/shaders/age_main_ui_fin_texture_shader.frag");

	aspects_ratio = float(resolution_h) / resolution_w;
	ver_pos_data[5] = 1.0f - 2 / aspects_ratio;
	ver_pos_data[7] = 1.0f - 2 / aspects_ratio;
	proj_mat = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	view_mat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	VP_mat = proj_mat * view_mat;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &FBO_texture);
	glBindTexture(GL_TEXTURE_2D, FBO_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution_w, resolution_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &RBO_depth_trapharet);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_depth_trapharet);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution_w, resolution_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_depth_trapharet);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // буфер трафарета не используется
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(2, vboIDs);
	glGenVertexArrays(1, &vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);

	UIObject();
}

void UIWindow::show()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // буфер трафарета не используется
	glEnable(GL_DEPTH_TEST);
	Camera::get_active_camera()->use_shader(&main_shader_prog_ui);

	for (std::uint16_t i = 0; i < childs.size(); i++)
		childs[i]->show_and_update(VP_mat, glm::mat4(1));

	Camera::get_active_camera()->use_main_shader();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	Camera::get_active_camera()->use_shader(&main_shader_fin_prog_ui);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, FBO_texture);
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





UIImage::UIImage()
{
	glGenBuffers(2, vboIDs);
	glGenVertexArrays(1, &vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIImage::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
{
	if (is_active) {
		glm::mat4 fin_model_mat = p_mat * model_mat;
		glm::mat4 fin_ui_mat = p_ui_mat * model_mat;

		for (std::uint16_t i = 0; i < childs_back.size(); i++)
			childs_back[i]->show_and_update(fin_model_mat, fin_ui_mat);

		Camera::get_active_camera()->set_model_matrix(&fin_model_mat);

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

		for (std::uint16_t i = 0; i < childs_front.size(); i++)
			childs_front[i]->show_and_update(fin_model_mat, fin_ui_mat);
	}
}

void UIImage::set_texture(Texture2D* tex)
{
	texture = tex;
	aspects_ratio = float(tex->get_height()) / tex->get_width();
	height = width * aspects_ratio / UIWindow::get_active_ui_window()->get_aspects_ratio();
	ver_pos_data[7] = 1.0f * aspects_ratio;
	ver_pos_data[10] = 1.0f * aspects_ratio;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIImage::set_z_val(float z)
{
	z_var = z;
	ver_pos_data[0] = z_var;
	ver_pos_data[3] = z_var;
	ver_pos_data[6] = z_var;
	ver_pos_data[9] = z_var;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (parent_obj != 0)
		parent_obj->_replace_backfront_child(this);
}

void UIImage::set_part_from_x_begin(float pc)
{
	shown_part_from_x_l = pc;
	ver_pos_data[5] = shown_part_from_x_l;
	ver_pos_data[8] = shown_part_from_x_l;
	tex_pos_data[2] = 1.0f - shown_part_from_x_l;
	tex_pos_data[4] = 1.0f - shown_part_from_x_l;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIImage::set_part_from_y_begin(float pc)
{
	shown_part_from_y_l = pc;
	ver_pos_data[1] = 1.0f - shown_part_from_y_l;
	ver_pos_data[4] = 1.0f - shown_part_from_y_l;
	tex_pos_data[5] = 1.0f + (1.0f - shown_part_from_y_l);
	tex_pos_data[7] = 1.0f + (1.0f - shown_part_from_y_l);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIImage::set_part_from_y_end(float pc)
{
	shown_part_from_y_e = pc;
	ver_pos_data[1] = (1.0f - shown_part_from_y_e) * aspects_ratio;
	ver_pos_data[4] = (1.0f - shown_part_from_y_e) * aspects_ratio;
	tex_pos_data[1] = 1.0f - shown_part_from_y_e;
	tex_pos_data[3] = 1.0f - shown_part_from_y_e;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void UIButton::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
{
	if (is_active) {
		UIImage::show_and_update(p_mat, p_ui_mat);

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
			glm::decompose(p_ui_mat * model_mat, scale_history, self_model_mat__orient_history, traslation_history, self_model_mat__skew_vec, self_model_mat__persp_history);

			if (mouse_pos_x < traslation_history.z + scale_history.z
				&& mouse_pos_x > traslation_history.z
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





void UIText::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
{
	if (is_active) {
		Camera::get_active_camera()->use_shader(Font::get_shader_program());
		GLuint shader_z_var_loc = glGetUniformLocation(Camera::get_active_camera()->get_active_shader()->get_shader_program_id(), "z_val");
		glUniform1f(shader_z_var_loc, z_var);

		glm::mat4 fin_mat = p_mat * model_mat;
		Camera::get_active_camera()->set_model_matrix(&fin_mat);

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
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), text_data, GL_DYNAMIC_DRAW);

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
	float width_max_line = 0.0f, width_now = 0.0f;
	for (int i = 0; i < str.size(); i++) {
		if (is_next_word_bigger_then_width(i, line_size_counter)) {
			str_eds_symbol_nums.push_back(i - 1);
			width_max_line = width_max_line > width_now ? width_max_line : width_now;
			width_now = 0.0f;
			line_size_counter = 0.0f;
		}
		while (str[i] != 10 && str[i] != 32 && str[i] != 0) {
			width_now += font->Characters[str[i]].Advance * font_t_size;
			line_size_counter += font->Characters[str[i]].Advance * font_t_size;
			i++;
		}
		if (str[i] == 10) {
			str_eds_symbol_nums.push_back(i);
			line_size_counter = 0.0f;
			width_max_line = width_max_line > width_now ? width_max_line : width_now;
			width_now = 0.0f;
			continue;
		}
		else if (str[i] == 32) {
			width_now += font->Characters[str[i]].Advance * font_t_size;
			line_size_counter += font->Characters[str[i]].Advance * font_t_size;
			if (line_size_counter > max_width) {
				str_eds_symbol_nums.push_back(i);
				width_max_line = width_max_line > width_now ? width_max_line : width_now;
				width_now = 0.0f;
				line_size_counter = 0.0f;
			}
			continue;
		}
	}
	width_max_line = width_max_line > width_now ? width_max_line : width_now;
	width = width_max_line;
}

bool UIText::is_next_word_bigger_then_width(int char_start_word, float now_str_width)
{
	while (str[char_start_word] != 32 && str[char_start_word] != 10 && str[char_start_word] != 0) {
		now_str_width += font->Characters[str[char_start_word]].Advance * font_t_size;
		char_start_word++;
	}
	if (now_str_width > max_width)
		return true;
	return false;
}

void UIText::set_z_val(float z)
{
	z_var = z;
	if (parent_obj != 0)
		parent_obj->_replace_backfront_child(this);
}

void UIText::set_string(std::string string)
{
	str = string;
	calculate_str_end_symbol_nums_w_words();
}





void UIOrganizedContainer::set_size(float w, float h)
{
	float new_scale = w / width;
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, new_scale, new_scale));
	width = w;
	height = h;
	aspects_ratio = height / width * parent_obj->get_aspects_ratio();
	model_mat = translate_mat * scale_mat * rotate_mat;
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

void UIOrganizedContainer::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
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

		//global_model_mat = UIWindow::get_active_ui_window()->get_ui_window_matrix() * list_translate_mat * m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		//model_mat_in_ui = list_translate_mat * m_mat_ui * (translate_mat * rotate_mat * scale_mat);
		glm::mat4 fin_matrix = list_translate_mat * p_mat * model_mat;
		glm::mat4 fin_ui_matrix = list_translate_mat * p_ui_mat * model_mat;
		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(fin_matrix, fin_ui_matrix);
	}
}





void UIContainer::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
{
	if (is_active) {
		for (int i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(p_mat * model_mat, p_ui_mat * model_mat);
	}
}

// IT'S NAY BE WRONG REALIZATION. MUST BE TESTED
void UIContainer::set_size(float w, float h)
{
	float new_scale_w = w / width;
	float new_scale_h = h / height;
	height = h;
	width = w;
	scale_mat = glm::scale(scale_mat, glm::vec3(1.0f, new_scale_w, new_scale_w));
	model_mat = translate_mat * scale_mat * rotate_mat;
	real_height = height * parent_obj->get_aspects_ratio();
	aspects_ratio = real_height / width;
}





UIVideo::~UIVideo()
{
	av_free(rgb_buffer);
	av_free(frameRGB);
	av_free(frame[0]);
	av_free(frame[1]);
	avcodec_close(codec_context[0]);
	avcodec_close(codec_context[1]);
	avformat_close_input(&format_context);
}

UIVideo::UIVideo()
{
	glGenBuffers(2, vboIDs);
	glGenVertexArrays(1, &vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	format_context = avformat_alloc_context();
	frame[0] = av_frame_alloc();
	frame[1] = av_frame_alloc();
	packet = av_packet_alloc();
	av_init_packet(packet);
	frameRGB = av_frame_alloc();
	
}

void UIVideo::show_and_update(glm::mat4 p_mat, glm::mat4 p_ui_mat)
{
	double now_time = ProgramClock::get_eleapsed_frame_time();
	double duration = (double)frame[video_stream_id]->pkt_duration * _time_base_ffmpeg;
	ms_passed_after_last_frame += now_time;

	if (ms_passed_after_last_frame >= _frame_rate_sec) {
		while (av_read_frame(format_context, packet) >= 0) {
			if (packet->stream_index == video_stream_id_in_context) {
				int ret = avcodec_send_packet(codec_context[video_stream_id], packet);
				if (ret < 0) {
					printf("AGE::ERROR::UIVideo::set_first_frame::Can't send video packet\n");
					break;
				}
				int ret2 = avcodec_receive_frame(codec_context[video_stream_id], frame[0]);
				if (frame[video_stream_id]->width > 0) {
					sws_scale(sws_cntx, frame[0]->data, frame[0]->linesize, 0, codec_context[video_stream_id]->height, frameRGB->data, frameRGB->linesize);

					glBindTexture(GL_TEXTURE_2D, video_texture);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame[video_stream_id]->width, frame[video_stream_id]->height,
						0, GL_RGB, GL_UNSIGNED_BYTE, rgb_buffer);

					break;
				}
			}
			av_packet_unref(packet);
		}
		if (av_read_frame(format_context, packet) < 0)
			restart_video();
		ms_passed_after_last_frame = ms_passed_after_last_frame - _frame_rate_sec;
	}

	if (is_active) {
		glm::mat4 fin_model_mat = p_mat * model_mat;
		glm::mat4 fin_ui_mat = p_ui_mat * model_mat;
		Camera::get_active_camera()->set_model_matrix(&fin_model_mat);

		glBindVertexArray(vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindTexture(GL_TEXTURE_2D, video_texture);
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

		for (std::uint16_t i = 0; i < childs.size(); i++)
			childs[i]->show_and_update(fin_model_mat, fin_ui_mat);
	}
}

int UIVideo::load_video_file(const char* file_name)
{
	if (avformat_open_input(&format_context, file_name, NULL, 0) != 0) {
		printf("AGE::ERROR::UIVideo::load_video_file::Can't open %s video file\n", file_name);
		return AGE_UI_VIDEO_OPEN_FILE_ERROR;
	}
	if (avformat_find_stream_info(format_context, NULL) < 0) {
		printf("AGE::ERROR::UIVideo::load_video_file::Can't find stream in %s video file\n", file_name);
		return AGE_UI_VIDEO_STREAM_NOT_FOUND;
	}

	for (uint16_t i = 0; i < format_context->nb_streams; i++) {
		AVCodecParameters* local_codec_param = format_context->streams[i]->codecpar;
		AVCodec* local_codec = avcodec_find_decoder(local_codec_param->codec_id);

		if (local_codec == NULL)
			continue;
		if (local_codec_param->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_stream_id_in_context = i;
			codec_parameters[video_stream_id] = local_codec_param;
			codec[video_stream_id] = local_codec;
			codec_context[video_stream_id] = avcodec_alloc_context3(codec[video_stream_id]);
			avcodec_parameters_to_context(codec_context[video_stream_id], codec_parameters[video_stream_id]);
			if (avcodec_open2(codec_context[video_stream_id], codec[video_stream_id], NULL) < 0) {
				printf("AGE::ERROR::UIVideo::load_video_file::Can't open appropriate video codec in %s video file\n", file_name);
				return AGE_UI_VIDEO_APPROPRIATE_VIDEO_CODEC_DOES_NOT_OPEN;
			}
		}
		else if (local_codec_param->codec_type == AVMEDIA_TYPE_AUDIO) {
			/*
			sound_stream_id_in_context = i;
			codec_parameters[sound_stream_id] = local_codec_param;
			codec[sound_stream_id] = local_codec;
			codec_context[sound_stream_id] = avcodec_alloc_context3(codec[video_stream_id]);
			avcodec_parameters_to_context(codec_context[sound_stream_id], codec_parameters[sound_stream_id]);
			if (avcodec_open2(codec_context[sound_stream_id], codec[sound_stream_id], NULL) < 0) {
				printf("AGE::ERROR::UIVideo::load_video_file::Can't open appropriate sound codec in %s video file\n", file_name);
				return AGE_UI_VIDEO_APPROPRIATE_SOUND_CODEC_DOES_NOT_OPEN;
			}*/
		}
	}

	if (codec_parameters[0] == 0) {
		printf("AGE::ERROR::UIVideo::load_video_file::Can't find video stream in %s video file\n", file_name);
		return AGE_UI_VIDEO_STREAM_NOT_FOUND_OR_UNSUPPORTED_CODEC;
	}
	avpicture_alloc((AVPicture*)frameRGB, AV_PIX_FMT_RGB24, codec_context[video_stream_id]->width, codec_context[video_stream_id]->height);
	rgb_buffer_num_bytes = avpicture_get_size(AV_PIX_FMT_RGB24, codec_context[video_stream_id]->width, codec_context[video_stream_id]->height);
	rgb_buffer = (uint8_t*)av_malloc(rgb_buffer_num_bytes * sizeof(uint8_t));
	avpicture_fill((AVPicture*)frameRGB, rgb_buffer, AV_PIX_FMT_RGB24, codec_context[video_stream_id]->width, codec_context[video_stream_id]->height);
	sws_cntx = sws_getContext(codec_context[video_stream_id]->width,
		codec_context[video_stream_id]->height,
		codec_context[video_stream_id]->pix_fmt,
		codec_context[video_stream_id]->width,
		codec_context[video_stream_id]->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	_time_base_ffmpeg = (double)format_context->streams[video_stream_id_in_context]->time_base.num / (double)format_context->streams[video_stream_id_in_context]->time_base.den;
	_frame_rate_sec = 2.0f / av_q2d(format_context->streams[video_stream_id_in_context]->avg_frame_rate);
	set_first_frame();

	aspects_ratio = float(codec_context[video_stream_id]->height) / codec_context[video_stream_id]->width;
	height = width * aspects_ratio / UIWindow::get_active_ui_window()->get_aspects_ratio();
	ver_pos_data[7] = 1.0f * aspects_ratio;
	ver_pos_data[10] = 1.0f * aspects_ratio;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ver_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIVideo::restart_video()
{
	int stream_id = av_find_best_stream(format_context, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	av_seek_frame(format_context, stream_id, 0, AVSEEK_FLAG_BACKWARD);
}

void UIVideo::set_first_frame()
{
	while (av_read_frame(format_context, packet) >= 0) {
		if (packet->stream_index == video_stream_id_in_context) {
			int ret = avcodec_send_packet(codec_context[video_stream_id], packet);
			if (ret < 0) {
				printf("AGE::ERROR::UIVideo::set_first_frame::Can't send video packet\n");
				break;
			}
			int ret2 = avcodec_receive_frame(codec_context[video_stream_id], frame[0]);
			if (frame[video_stream_id]->width > 0) {
				sws_scale(sws_cntx, frame[0]->data, frame[0]->linesize, 0, codec_context[video_stream_id]->height, frameRGB->data, frameRGB->linesize);

				glBindTexture(GL_TEXTURE_2D, video_texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame[video_stream_id]->width, frame[video_stream_id]->height,
					0, GL_RGB, GL_UNSIGNED_BYTE, rgb_buffer);
				glBindTexture(GL_TEXTURE_2D, 0);

				break;
			}
		}
		av_packet_unref(packet);
	}
}
