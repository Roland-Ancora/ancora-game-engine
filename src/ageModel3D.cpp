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

void Model3D::set_position(float x, float y, float z)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x - x_pos, y - y_pos, z - z_pos));
	x_pos = x, y_pos = y, z_pos = z;
}

void Model3D::move(float x, float y, float z)
{
	translate_mat = glm::translate(translate_mat, glm::vec3(x, y, z));
	x_pos += x, y_pos += y, z_pos += z;
}

void Model3D::set_rotation(float angle, rotate_vector vec)
{
	glm::mat4 rotate_by_vector_now = glm::rotate(glm::mat4(1), angle, vec);
	rotate_mat *= rotate_by_vector_now;
}

void Model3D::rotate(float angle, rotate_vector vec)
{
	rotate_mat = glm::rotate(rotate_mat, angle, vec);
}






Model3dGroupNode::~Model3dGroupNode()
{
	delete[] childs;
}

void Model3dGroupNode::show()
{
	model.show();
	for (int i = 0; i < childs_count; i++)
		childs[i].show();
}

void Model3dGroupNode::set_from_data_node(Model3dGroupDataNode* node)
{
	model.set_model_3d_data(&node->model);
	model.set_position(node->position.x, node->position.y, node->position.z);
	model.set_rotation(-node->rotation.x, AGE_ROTATE_AROUND_X); // - because OpenGL rotates object counterclock-wise
	model.set_rotation(-node->rotation.y, AGE_ROTATE_AROUND_Y);
	model.set_rotation(-node->rotation.z, AGE_ROTATE_AROUND_Z);

	childs_count = node->childs_count;
	childs = new Model3dGroupNode[childs_count];
	for (int i = 0; i < childs_count; i++)
		childs[i].set_from_data_node(&node->childs[i]);
}





Model3dGroup::~Model3dGroup()
{
	delete[] childs;
}

void Model3dGroup::show()
{
	for (int i = 0; i < childs_count; i++)
		childs[i].show();
}

void Model3dGroup::set_group_from_data(Model3dGroupData* data)
{
	childs = new Model3dGroupNode[data->childs_count];
	childs_count = data->childs_count;
	for (int i = 0; i < childs_count; i++)
		childs[i].set_from_data_node(&data->childs[i]);
}
