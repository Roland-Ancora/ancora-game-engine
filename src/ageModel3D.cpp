#include "../include/ageModel3D.h"



using namespace age;



void Model3D::show()
{
	glm::mat4 test_finaly_mat = translate_mat * rotate_mat * scale_mat;
	Camera::get_active_camera()->set_model_matrix(&test_finaly_mat);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, model_data->diffuse_texture);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, model_data->vertices);
	glNormalPointer(GL_FLOAT, 0, model_data->normals);
	glTexCoordPointer(2, GL_FLOAT, 0, model_data->texture_coords);
	glDrawElements(GL_TRIANGLES, model_data->indices_cnt, GL_UNSIGNED_INT, model_data->indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}
