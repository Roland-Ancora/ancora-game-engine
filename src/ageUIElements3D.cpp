#include "../include/ageUIElements3D.h"



using namespace age;



ShaderProgram UIElement3D::ui3d_sh_prog;



void UIElement3D::Init()
{
	ui3d_sh_prog = ShaderProgram::create_shader_program(AGE_DEFAULT_3D_UI3D_VERTEX_SHADER, AGE_DEFAULT_3D_UI3D_FRAGMENT_SHADER);
}




void UIImage3D::show()
{
	Camera::get_active_camera()->use_shader(&ui3d_sh_prog);
	if (camera_follow)
		set_rotation(Camera3D::get_active_3d_camera()->get_rotation().x, AGE_ROTATE_AROUND_Y);
	finally_mat = translate_mat * rotate_mat * scale_mat;
	if (parent_element != 0)
		finally_mat = parent_element->_get_model_matrix() * finally_mat;
	Camera::get_active_camera()->set_model_matrix(&finally_mat);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vert_tex_pos_data, GL_STATIC_DRAW);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, *img_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vaoID);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	Camera::get_active_camera()->use_main_shader();
}

void UIImage3D::set_texture(Texture2D* texture)
{
	img_texture = texture;
	width = texture->get_width() / 100; // without / 100 it's will be very big
	height = texture->get_height() / 100;
	vert_tex_pos_data[0] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[4] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[8] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[12] = -(width * shown_part_from_x_begin / 2);
}

void UIImage3D::set_size(float w, float h)
{
	width = w;
	height = h;
	vert_tex_pos_data[0] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[1] = height / 2;
	vert_tex_pos_data[4] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[5] = height / 2;
	vert_tex_pos_data[8] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[9] = -(height / 2);
	vert_tex_pos_data[12] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[13] = -(height / 2);
}

void UIImage3D::set_size(float w)
{
	width = w;
	height = w * (float(img_texture->get_height()) / img_texture->get_width());
	vert_tex_pos_data[0] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[1] = height / 2;
	vert_tex_pos_data[4] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[5] = height / 2;
	vert_tex_pos_data[8] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[9] = -(height / 2);
	vert_tex_pos_data[12] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[13] = -(height / 2);
}

void UIImage3D::set_position(float x, float y, float z)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x - x_pos, y - y_pos, z - z_pos));
	x_pos = x, y_pos = y, z_pos = z;
}

void UIImage3D::set_rotation(float angle, rotate_vector vec)
{
	glm::mat4 rotate_by_vector_now = glm::rotate(glm::mat4(1), angle, vec);
	rotate_mat = rotate_by_vector_now;
	if (middle_rotate_point_active) {
		glm::mat4 transl_aft_rot = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, rotate_arm));
		rotate_mat = rotate_mat * transl_aft_rot;
	}
}

void UIImage3D::rotate(float angle, rotate_vector vec)
{
	rotate_mat = glm::rotate(rotate_mat, angle, vec);
	if (middle_rotate_point_active) {
		glm::mat4 transl_aft_rot = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, rotate_arm));
		rotate_mat = rotate_mat * transl_aft_rot;
	}
}

void UIImage3D::set_part_from_x_begin(float pc)
{
	shown_part_from_x_begin = pc;
	vert_tex_pos_data[0] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[4] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[8] = width * shown_part_from_x_begin / 2;
	vert_tex_pos_data[12] = -(width * shown_part_from_x_begin / 2);
	vert_tex_pos_data[5] = 1.0f + (1.0f - shown_part_from_x_begin);
	vert_tex_pos_data[9] = 1.0f + (1.0f - shown_part_from_x_begin);
}

void UIImage3D::use_middle_rotate_point(float arm)
{
	middle_rotate_point_active = true;
	rotate_arm = arm;
	glm::mat4 transl_aft_rot = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, rotate_arm));
	rotate_mat = rotate_mat * transl_aft_rot;
}

void UIImage3D::disable_middle_rotate_point()
{
	middle_rotate_point_active = false;
	rotate_mat = rotate_mat * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -rotate_arm));
}

void UIImage3D::set_arm(float arm)
{
	rotate_mat = rotate_mat * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, arm - rotate_arm));
	rotate_arm = arm;
}




void UIButton3D::show()
{
	if (is_active) {
		UIImage3D::show();

		if (is_btn_active) {
			glm::vec2 cursor_pos = Window::get_active_window()->get_cursor_position();
			glm::vec3 mouse_pos_in_world = Camera3D::get_active_3d_camera()->get_pixel_position_in_world_coords(cursor_pos.x, cursor_pos.y, Window::get_active_window()->get_height());
			
			glm::vec4 mouse_pos_in_world_vec(mouse_pos_in_world[0], mouse_pos_in_world[1], mouse_pos_in_world[2], 1.0f);
			glm::mat4 inverse_finally_mat = glm::inverse(finally_mat);
			mouse_pos_in_world_vec = inverse_finally_mat * mouse_pos_in_world_vec;
			
			if (mouse_pos_in_world_vec.y > 0.0f && mouse_pos_in_world_vec.y < height &&
			mouse_pos_in_world_vec.x > 0.0f && mouse_pos_in_world_vec.x < width &&
			mouse_pos_in_world_vec.z < 0.01f && mouse_pos_in_world_vec.z > -0.01) {
				img_texture = focus_texture;

				if (is_btn_pressed)
					is_btn_pressed = false;
				if (InputEventsControler::is_mouse_button_pressed_and_released(GLFW_MOUSE_BUTTON_LEFT))
					is_btn_pressed = true;
			}
			else
				img_texture = default_texture;
		}
		else
			is_btn_pressed = false;
	}
	else
		is_btn_pressed = false;
}
