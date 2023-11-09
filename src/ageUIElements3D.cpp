#include "../include/ageUIElements3D.h"



using namespace age;



void UIImage3D::show()
{
	finally_mat = translate_mat * rotate_mat * scale_mat;
	Camera::get_active_camera()->set_model_matrix(&finally_mat);

	float vert_pos[] = {
		0.0f, height, 0.0f,
		width * shown_part_from_x_begin, height, 0.0f,
		width * shown_part_from_x_begin, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	float tex_pos[] = {
		0.0f, 0.0f,
		1.0f + (1.0f - shown_part_from_x_begin), 0.0f,
		1.0f + (1.0f - shown_part_from_x_begin), 1.0f,
		0.0f, 1.0f
	};
	float vert_normals[] = {
		0.0f, height, -1.0f,
		width, height, -1.0f,
		width, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};

	GLuint shader_color_loc_var = glGetUniformLocation(Camera::get_active_camera()->get_active_shader()->get_shader_program_id(), "object_color");
	glUniform3f(shader_color_loc_var, 1.0f, 1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vert_pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vert_normals, GL_STATIC_DRAW);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void UIImage3D::set_texture(Texture2D* texture)
{
	img_texture = texture;
	width = texture->get_width() / 100; // without / 100 it's will be very big
	height = texture->get_height() / 100;
}

void UIImage3D::set_size(float w, float h)
{
	width = w;
	height = h;
}

void UIImage3D::set_size(float w)
{
	width = w;
	height = w * (float(img_texture->get_height()) / img_texture->get_width());
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
}

void UIImage3D::rotate(float angle, rotate_vector vec)
{
	rotate_mat = glm::rotate(rotate_mat, angle, vec);
}




void UIButton3D::show()
{
	if (is_active) {
		UIImage3D::show();

		if (is_btn_active) {
			double* mouse_pos_in_wodtd = get_click_position_in_world_coords(*Window::get_active_window(), *Camera3D::get_active_3d_camera());

			
			glm::vec4 mouse_pos_in_world_vec(mouse_pos_in_wodtd[0], mouse_pos_in_wodtd[1], mouse_pos_in_wodtd[2], 1.0f);
			glm::mat4 inverse_finally_mat = glm::inverse(finally_mat);
			mouse_pos_in_world_vec = inverse_finally_mat * mouse_pos_in_world_vec;
			
			if (mouse_pos_in_world_vec.y > 0.0f && mouse_pos_in_world_vec.y < height &&
			mouse_pos_in_world_vec.x > 0.0f && mouse_pos_in_world_vec.x < width &&
			mouse_pos_in_world_vec.z < 0.01f && mouse_pos_in_world_vec.z > -0.01) {
				img_texture = focus_texture;

				if (is_btn_pressed)
					is_btn_pressed = false;
				if (InputEventsControler::is_mouse_button_pressed_and_released(*Window::get_active_window(), GLFW_MOUSE_BUTTON_LEFT))
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
