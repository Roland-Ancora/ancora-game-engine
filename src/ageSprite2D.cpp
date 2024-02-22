#include "../include/ageSprite2D.h"



using namespace age;



float Sprite2D::vert_pos_data[8] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f
};
float Sprite2D::tex_pos_data[8] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};



Sprite2D::Sprite2D()
{
	glGenBuffers(2, vboIDs); glGenVertexArrays(1, &vaoID);
}

void Sprite2D::set_texture(Texture2D* tex)
{
	texture = tex;
	aspect_ratio = float(tex->get_height()) / tex->get_width();
}

void Sprite2D::show()
{
	model_mat = translate_mat * rotate_mat * scale_mat;
	Camera::get_active_camera()->set_model_matrix(&model_mat);

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vert_pos_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tex_pos_data, GL_STATIC_DRAW);

	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
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
}

void Sprite2D::move(float x, float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x, y, 0.0f));
}

void Sprite2D::move_x(float x)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x, 0.0f, 0.0f));
}

void Sprite2D::move_y(float y)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(0.0f, y, 0.0f));
}
