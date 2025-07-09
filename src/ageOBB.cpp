#include "ageOBB.h"



using namespace age;



bool OBB::is_collide(OBB obb)
{
	for (short i = 0; i < 8; i++) {
		glm::vec3 point_cur = r_mat * obb.o_mat * glm::vec4(obb.points[i], 1.0f);
		if (point_cur.y > points[0].y || point_cur.y < points[3].y)
			continue;
		if (point_cur.x < points[0].x || point_cur.x > points[1].x)
			continue;
		if (point_cur.z < points[4].z || point_cur.z > points[0].z)
			continue;
		return true;
	}
	return false;
}

bool OBB::is_collide(glm::vec3 point)
{
	glm::vec3 point_cur = r_mat * glm::vec4(point, 1.0f);
	if (point_cur.y > points[0].y || point_cur.y < points[3].y ||
		point_cur.x < points[0].x || point_cur.x > points[1].x ||
		point_cur.z < points[4].z || point_cur.z > points[0].z)
		return false;
	return true;
}

void OBB::set_points(glm::vec3 s_point, glm::vec3 sides)
{
	points[OBB_SIDE_ULF] = glm::vec3(s_point.x, s_point.y + sides.y, s_point.z + sides.z);
	points[OBB_SIDE_URF] = glm::vec3(s_point.x + sides.x, s_point.y + sides.y, s_point.z + sides.z);
	points[OBB_SIDE_DRF] = glm::vec3(s_point.x + sides.x, s_point.y, s_point.z + sides.z);
	points[OBB_SIDE_DLF] = glm::vec3(s_point.x, s_point.y, s_point.z + sides.z);
	points[OBB_SIDE_ULB] = glm::vec3(s_point.x, s_point.y + sides.y, s_point.z);
	points[OBB_SIDE_URB] = glm::vec3(s_point.x + sides.x, s_point.y + sides.y, s_point.z);
	points[OBB_SIDE_DRB] = glm::vec3(s_point.x + sides.x, s_point.y, s_point.z);
	points[OBB_SIDE_DLB] = glm::vec3(s_point.x, s_point.y, s_point.z);
}

void OBB::set_from_model_data(OBB& obb, Model3dData* model_data)
{
	float m_point_x = model_data->vertices[0];
	float m_point_y = model_data->vertices[1];
	float m_point_z = model_data->vertices[2];
	float x2 = model_data->vertices[0];
	float y2 = model_data->vertices[1];
	float z2 = model_data->vertices[2];
	for (unsigned int i = 3; i < model_data->vertices_cnt; i += 0) {
		if (m_point_x > model_data->vertices[i])
			m_point_x = model_data->vertices[i];
		if (m_point_y > model_data->vertices[i + 1])
			m_point_y = model_data->vertices[i + 1];
		if (m_point_z < model_data->vertices[i + 2])
			m_point_z = model_data->vertices[i + 2];

		if (x2 < model_data->vertices[i])
			x2 = model_data->vertices[i];
		if (y2 < model_data->vertices[i + 1])
			y2 = model_data->vertices[i + 1];
		if (z2 > model_data->vertices[i + 2])
			z2 = model_data->vertices[i + 2];

		i += 3;
	}
	obb.points[OBB_SIDE_ULF] = glm::vec3(m_point_x, y2, m_point_z);
	obb.points[OBB_SIDE_URF] = glm::vec3(x2, y2, m_point_z);
	obb.points[OBB_SIDE_DRF] = glm::vec3(x2, m_point_y, m_point_z);
	obb.points[OBB_SIDE_DLF] = glm::vec3(m_point_x, m_point_y, m_point_z);
	obb.points[OBB_SIDE_ULB] = glm::vec3(m_point_x, y2, z2);
	obb.points[OBB_SIDE_URB] = glm::vec3(x2, y2, z2);
	obb.points[OBB_SIDE_DRB] = glm::vec3(x2, m_point_y, z2);
	obb.points[OBB_SIDE_DLB] = glm::vec3(m_point_x, m_point_y, z2);
}





void OBB_Model::set_from_model(OBB_Model& obb, Model3D* model3d)
{
	OBB::set_from_model_data(obb, model3d->get_model_data());
	obb.model = model3d;
	obb.o_mat = model3d->get_model_matrix();
	obb.r_mat = model3d->get_reverse_matrix();
}

void OBB_Model::update_matrices()
{
	o_mat = model->get_model_matrix();
	r_mat = model->get_reverse_matrix();
}





void OBB_Model_Group::update_matrices()
{
	for (int i = 0; i < obbs_cnt; i++) {
		obbs[i].o_mat = model->get_model_matrix();
		obbs[i].r_mat = model->get_reverse_matrix();
	}
}

void OBB_Model_Group::set_from_model_group(OBB_Model_Group& obb, Model3dGroup* model_group)
{
	obb.obbs_cnt = model_group->get_childs_count();
	obb.obbs = new OBB_Model[obb.obbs_cnt];
	for (int i = 0; i < model_group->get_childs_count(); i++) {
		obb.obbs[i].model = model_group->get_child(i)->get_model();
		OBB::set_from_model_data(obb.obbs[i], obb.obbs[i].model->get_model_data());
		obb.obbs[i].r_mat = obb.obbs[i].model->get_reverse_matrix();
		obb.obbs[i].o_mat = obb.obbs[i].model->get_model_matrix();
	}
}

bool OBB_Model_Group::is_collide(OBB obb)
{
	for (int i = 0; i < obbs_cnt; i++)
		if (obbs[i].is_collide(obb))
			return true;
	return false;
}

bool OBB_Model_Group::is_collide(glm::vec3 point)
{
	for (int i = 0; i < obbs_cnt; i++)
		if (obbs[i].is_collide(point))
			return true;
	return false;
}
