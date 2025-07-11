#include "ageModel3D.h"



using namespace age;



void Model3D::opengl_draw()
{
	GLuint c_loc = glGetUniformLocation(Camera::get_active_camera()->get_active_shader()->get_shader_program_id(), "color");
	GLuint ci_loc = glGetUniformLocation(Camera::get_active_camera()->get_active_shader()->get_shader_program_id(), "color_intensity");
	glUniform3f(c_loc, color.b, color.g, color.b);
	glUniform1f(ci_loc, color_intensity);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model_data->indices_cnt * sizeof(unsigned int), model_data->indices, GL_STATIC_DRAW);

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

	glBindTexture(GL_TEXTURE_2D, model_data->diffuse_texture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[3]);

	glDrawElements(
		GL_TRIANGLES,
		model_data->indices_cnt,
		GL_UNSIGNED_INT,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Model3D::show()
{
	glm::mat4 finaly_mat;
	if (ALTR_fin_model_mat == glm::mat4(0))
		finaly_mat = translate_mat * rotate_mat * scale_mat;
	else
		finaly_mat = ALTR_fin_model_mat;
	Camera::get_active_camera()->set_model_matrix(&finaly_mat);
	opengl_draw();
}

void Model3D::set_model_3d_data(Model3dData* model_3d_data)
{
	model_data = model_3d_data;
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, model_data->vertices_cnt * sizeof(float), model_data->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, model_data->texture_coords_cnt * sizeof(float), model_data->texture_coords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, model_data->normals_cnt * sizeof(float), model_data->normals, GL_STATIC_DRAW);
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
	rotate_mat = rotate_by_vector_now;

	if (vec == AGE_ROTATE_AROUND_X)
		x_rot = angle;
	else if (vec == AGE_ROTATE_AROUND_Y)
		y_rot = angle;
	else
		z_rot = angle;
}

void Model3D::set_rotation(glm::mat4 matrix)
{
	rotate_mat = matrix;
	glm::vec3 _tmp_vec3;
	glm::vec4 _persp;
	glm::quat _rotate;
	glm::decompose(matrix, _tmp_vec3, _rotate, _tmp_vec3, _tmp_vec3, _persp);
	x_rot = _rotate.x, y_rot = _rotate.y, z_rot = _rotate.z;
}

void Model3D::rotate(float angle, rotate_vector vec)
{
	if (vec == AGE_ROTATE_AROUND_X)
		x_rot += angle;
	else if (vec == AGE_ROTATE_AROUND_Y)
		y_rot += angle;
	else
		z_rot += angle;

	glm::quat q = glm::yawPitchRoll(x_rot, y_rot, z_rot);
	rotate_mat = glm::toMat4(q);
}

void Model3D::set_scale(float x, float y, float z)
{
	scale_mat = glm::scale(scale_mat, glm::vec3(x / x_scale, y / y_scale, z / z_scale));
	x_scale = x, y_scale = y, z_scale = z;
}

glm::mat4 Model3D::get_reverse_matrix()
{
	glm::mat4 mat = glm::mat4(1);
	mat = glm::scale(mat, glm::vec3(1 / x_scale, 1 / y_scale, 1 / z_scale));
	mat = glm::rotate(mat, -x_rot, AGE_ROTATE_AROUND_X);
	mat = glm::rotate(mat, -y_rot, AGE_ROTATE_AROUND_Y);
	mat = glm::rotate(mat, -z_rot, AGE_ROTATE_AROUND_Z);
	mat = glm::translate(mat, glm::vec3(-x_pos, -y_pos, -z_pos));
	return ALTR_fin_model_mat == glm::mat4(0) ? mat : glm::inverse(ALTR_fin_model_mat);
}





void Model3D_Anim::update_anim_matrix()
{
	int prev_node = anim_last_node - 1;
	float time_keys_vals_residual = static_cast<float>(anim->nodes[anim_last_node].time_val - anim->nodes[prev_node].time_val);
	float time_progress_btw_nodes = static_cast<float>((now_play_time - anim->nodes[prev_node].time_val) / time_keys_vals_residual);
	
	// Position calculation
	float vals_keys_pos_x_residual = anim->nodes[anim_last_node].position[0] - anim->nodes[prev_node].position[0];
	float vals_keys_pos_y_residual = anim->nodes[anim_last_node].position[1] - anim->nodes[prev_node].position[1];
	float vals_keys_pos_z_residual = anim->nodes[anim_last_node].position[2] - anim->nodes[prev_node].position[2];
	float val_pos_x_done = vals_keys_pos_x_residual * time_progress_btw_nodes;
	float val_pos_y_done = vals_keys_pos_y_residual * time_progress_btw_nodes;
	float val_pos_z_done = vals_keys_pos_z_residual * time_progress_btw_nodes;
	glm::vec3 pos_in_anim(anim->nodes[prev_node].position[0] + val_pos_x_done,
							anim->nodes[prev_node].position[1] + val_pos_y_done,
							anim->nodes[prev_node].position[2] + val_pos_z_done);
	anim_matrix = glm::translate(anim_matrix, pos_in_anim);

	// Rotation calculation
	float vals_keys_rot_x_residual = anim->nodes[anim_last_node].rotation[0] - anim->nodes[prev_node].rotation[0];
	float vals_keys_rot_y_residual = anim->nodes[anim_last_node].rotation[1] - anim->nodes[prev_node].rotation[1];
	float vals_keys_rot_z_residual = anim->nodes[anim_last_node].rotation[2] - anim->nodes[prev_node].rotation[2];
	float val_rot_x_done = vals_keys_rot_x_residual * time_progress_btw_nodes;
	float val_rot_y_done = vals_keys_rot_y_residual * time_progress_btw_nodes;
	float val_rot_z_done = vals_keys_rot_z_residual * time_progress_btw_nodes;

	anim_matrix = glm::rotate(anim_matrix, anim->nodes[prev_node].rotation[0] + val_rot_x_done, glm::vec3(1, 0, 0));
	anim_matrix = glm::rotate(anim_matrix, anim->nodes[prev_node].rotation[1] + val_rot_y_done, glm::vec3(0, 1, 0));
	anim_matrix = glm::rotate(anim_matrix, anim->nodes[prev_node].rotation[2] + val_rot_z_done, glm::vec3(0, 0, 1));

	// Scale calculation
	float vals_keys_scale_x_residual = anim->nodes[anim_last_node].scaling[0] - anim->nodes[prev_node].scaling[0];
	float vals_keys_scale_y_residual = anim->nodes[anim_last_node].scaling[1] - anim->nodes[prev_node].scaling[1];
	float vals_keys_scale_z_residual = anim->nodes[anim_last_node].scaling[2] - anim->nodes[prev_node].scaling[2];
	float val_scale_x_done = time_progress_btw_nodes * vals_keys_scale_x_residual + anim->nodes[prev_node].scaling[0];
	float val_scale_y_done = time_progress_btw_nodes * vals_keys_scale_y_residual + anim->nodes[prev_node].scaling[1];
	float val_scale_z_done = time_progress_btw_nodes * vals_keys_scale_z_residual + anim->nodes[prev_node].scaling[2];;
	// So, I don't now why I have some problem with export model to FBX format and why I should to change z and y scale
	anim_matrix = glm::scale(anim_matrix, glm::vec3(val_scale_x_done, val_scale_z_done, val_scale_y_done));
}

void Model3D_Anim::show()
{
	anim_matrix = translate_mat * rotate_mat * scale_mat;

	if (anim_play) {
		now_play_time += ProgramClock::get_eleapsed_frame_time();
		if (now_play_time >= anim->anim_duration) {
			if (is_anim_loop) {
				now_play_time = now_play_time - anim->anim_duration;
				anim_last_node = 0;
			}
			else {
				now_play_time = 0.0f;
				anim_last_node = 0;
				anim_play = false;
			}
		}
		while (anim->nodes[anim_last_node].time_val < now_play_time)
			anim_last_node++;
		if (anim_play)
			update_anim_matrix();
	}

	Camera::get_active_camera()->set_model_matrix(&anim_matrix);
	opengl_draw();
	for (int i = 0; i < childs.size(); i++)
		childs[i]->show_child(anim_matrix);
}

void Model3D_Anim::show_model_only()
{
	anim_matrix = translate_mat * rotate_mat * scale_mat;
	Camera::get_active_camera()->set_model_matrix(&anim_matrix);
	opengl_draw();
	for (int i = 0; i < childs.size(); i++)
		childs[i]->show_child_model_only(anim_matrix);
}





void Model3D_Anim__child::show_child(glm::mat4 parent_mat)
{
	anim_matrix = parent_mat * (translate_mat * rotate_mat * scale_mat);

	if (anim_play) {
		now_play_time += ProgramClock::get_eleapsed_frame_time();
		if (now_play_time >= anim->anim_duration) {
			if (is_anim_loop) {
				now_play_time = now_play_time - anim->anim_duration;
				anim_last_node = 0;
			}
			else {
				now_play_time = 0.0f;
				anim_last_node = 0;
				anim_play = false;
			}
		}
		while (anim->nodes[anim_last_node].time_val < now_play_time)
			anim_last_node++;
		if (anim_play)
			update_anim_matrix();
	}

	Camera::get_active_camera()->set_model_matrix(&anim_matrix);
	opengl_draw();
}

void Model3D_Anim__child::show_child_model_only(glm::mat4 parent_mat)
{
	anim_matrix = parent_mat * (translate_mat * rotate_mat * scale_mat);
	Camera::get_active_camera()->set_model_matrix(&anim_matrix);
	opengl_draw();
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
	if (node->ALTR_fin_model_mat == glm::mat4(0)) {
		model.set_position(node->position.x, node->position.y, node->position.z);
		model.rotate(node->rotation.y, AGE_ROTATE_AROUND_Y);
		model.rotate(node->rotation.x, AGE_ROTATE_AROUND_X);
		model.rotate(node->rotation.z, AGE_ROTATE_AROUND_Z);
		model.rotate(node->rotation.x, AGE_ROTATE_AROUND_X);// �������� � ������ ������, ��� ������������ ������ ��� ��������0
		model.set_only_rotation_mat(glm::toMat4(node->rotation_quat));
		model.set_scale(node->scale.x, node->scale.y, node->scale.z);
	}
	else
		model.set_alternative_finnaly_matrix(node->ALTR_fin_model_mat);

	childs_count = node->childs_count;
	childs = new Model3dGroupNode[childs_count];
	for (int i = 0; i < childs_count; i++)
		childs[i].set_from_data_node(&node->childs[i]);
}





Model3dGroup::~Model3dGroup()
{
	delete[] childs;
}

void Model3dGroup::set_position(glm::vec3 pos)
{
	model_matrix = glm::translate(glm::mat4(1), pos);
	for (uint16_t i = 0; i < childs_count; i++)
		childs[i].get_model()->set_position(pos.x, pos.y, pos.z);
}

void Model3dGroup::set_scale(glm::vec3 scl)
{
	scale = scl;
	for (uint16_t i = 0; i < childs_count; i++)
		childs[i].model.set_scale(scale.x, scale.y, scale.z);
}

void Model3dGroup::set_group_from_data(Model3dGroupData* data)
{
	childs = new Model3dGroupNode[data->childs_count];
	childs_count = data->childs_count;
	for (int i = 0; i < childs_count; i++)
		childs[i].set_from_data_node(&data->childs[i]);
}

void Model3dGroup::show()
{
	for (int i = 0; i < childs_count; i++)
		childs[i].show();
}

void Model3dGroup::clear()
{
	childs_count = 0;
	delete[] childs;
}
