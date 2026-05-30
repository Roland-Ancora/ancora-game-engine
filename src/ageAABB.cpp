#include "ageAABB.h"



using namespace age;



collide_hit_answer OBB::is_collide(OBB obb)
{
	collide_hit_answer hit;

	glm::vec3 otherMin(FLT_MAX);
	glm::vec3 otherMax(-FLT_MAX);

	for (int i = 0; i < 8; i++)
	{
		glm::vec3 corner = glm::vec3(
			(i & 1) ? obb.maxP.x : obb.minP.x,
			(i & 2) ? obb.maxP.y : obb.minP.y,
			(i & 4) ? obb.maxP.z : obb.minP.z
		);

		glm::vec3 p = r_mat * obb.o_mat * glm::vec4(corner, 1.0f);

		otherMin = glm::min(otherMin, p);
		otherMax = glm::max(otherMax, p);
	}

	if (maxP.x < otherMin.x || minP.x > otherMax.x) { return hit; }
	if (maxP.y < otherMin.y || minP.y > otherMax.y) { return hit; }
	if (maxP.z < otherMin.z || minP.z > otherMax.z) { return hit; }

	glm::vec3 aMin = glm::min(minP, maxP);
	glm::vec3 aMax = glm::max(minP, maxP);

	glm::vec3 bMin = glm::min(otherMin, otherMax);
	glm::vec3 bMax = glm::max(otherMin, otherMax);

	glm::vec3 overlap;
	overlap.x = std::min(maxP.x - otherMin.x, otherMax.x - aMin.x);
	overlap.y = 100;
	overlap.z = std::min(maxP.z - otherMin.z, otherMax.z - aMin.z);

	glm::vec3 Acenter = (aMin + maxP) * 0.5f;
	glm::vec3 worldCenter = obb.o_mat * glm::vec4((bMin + bMax) * 0.5f, 1.0f);
	glm::vec3 Bcenter = r_mat * glm::vec4(worldCenter, 1.0f);

	if (overlap.x < overlap.y && overlap.x < overlap.z) {
		float pushFromNegZ = bMax.x - aMin.x;
		float pushFromPosZ = aMax.x - bMin.x;
		if (pushFromNegZ < pushFromPosZ)
		{
			hit.penetration = pushFromNegZ;
			hit.normal = glm::vec3(-1, 0, 0);
		}
		else
		{
			hit.penetration = pushFromPosZ;
			hit.normal = glm::vec3(1, 0, 0);
		}
	}
	else if (overlap.y < overlap.z) {
		hit.penetration = overlap.y;
		hit.normal = (Acenter.y < Bcenter.y) ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0);
	}
	else {
		float pushFromNegZ = bMax.z - aMin.z;
		float pushFromPosZ = aMax.z - bMin.z;
		if (pushFromNegZ < pushFromPosZ)
		{
			hit.penetration = pushFromNegZ;
			hit.normal = glm::vec3(0, 0, -1);
		}
		else
		{
			hit.penetration = pushFromPosZ;
			hit.normal = glm::vec3(0, 0, 1);
		}
	}

	hit.collided = true;
	return hit;
}

collide_hit_answer OBB::is_collide(glm::vec3 point)
{
	collide_hit_answer hit;
	return hit;
}


void OBB::set_points(glm::vec3 s_point, glm::vec3 sides)
{
	minP = s_point;
	maxP.x = minP.x + sides.x, maxP.y = minP.y + sides.y, maxP.z = minP.z + sides.z;
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
		if (m_point_z > model_data->vertices[i + 2])
			m_point_z = model_data->vertices[i + 2];

		if (x2 < model_data->vertices[i])
			x2 = model_data->vertices[i];
		if (y2 < model_data->vertices[i + 1])
			y2 = model_data->vertices[i + 1];
		if (z2 < model_data->vertices[i + 2])
			z2 = model_data->vertices[i + 2];

		i += 3;
	}
	obb.minP.x = m_point_x, obb.minP.y = m_point_y, obb.minP.z = m_point_z;
	obb.maxP.x = x2, obb.maxP.y = y2, obb.maxP.z = z2;
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

collide_hit_answer OBB_Model_Group::is_collide(OBB obb)
{
	collide_hit_answer hit;
	for (int i = 0; i < obbs_cnt; i++) {
		hit = obbs[i].is_collide(obb);
		if (hit.collided)
			return hit;
	}
	return hit;
}

collide_hit_answer OBB_Model_Group::is_collide(glm::vec3 point)
{
	collide_hit_answer hit;
	for (int i = 0; i < obbs_cnt; i++) {
		hit = obbs[i].is_collide(point);
		if (hit.collided)
			return hit;
	}
	return hit;
}
