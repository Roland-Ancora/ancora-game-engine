#include "ageCollidesControler.h"



using namespace age;



CollidesControler* CollidesControler::active_singltone = 0;

CollidesControler::CollidesControler()
{
	if (active_singltone != 0)
		exit(AGE_COLLIDES_CONTROLER_OBJECT_OVERLAPPING_ERROR);
	active_singltone = this;
}

void CollidesControler::add_obb_by_type(std::vector<OBB*>* vect, obb_type type)
{
	switch (type)
	{
	case OBB_OBB:
		vect->push_back(new OBB());
		break;
	case OBB_MODEL:
		vect->push_back(new OBB_Model());
		break;
	case OBB_MODEL_GROUP:
		vect->push_back(new OBB_Model_Group());
		break;
	default:
		break;
	}
}

int CollidesControler::add_obb(OBB* obb, obb_controler_element_type type_el, obb_type type_obb)
{
	if (type_el == OBB_DYNAMIC) {
		dynamic_obbs.push_back(dynamic_obb_struct(obb));
		return dynamic_obbs.size() - 1;
	}
	add_obb_by_type(&static_obbs, type_obb);
	static_obbs[static_obbs.size() - 1] = obb;
	return static_obbs.size() - 1;
}

collide_obb_answer CollidesControler::test_obb_with_statics(OBB* obb)
{
	collide_obb_answer ret;
	for (int i = 0; i < static_obbs.size(); i++) {
		if (obb->is_collide(*static_obbs[i]) || static_obbs[i]->is_collide(*obb)) {
			ret.collide = true;
			return ret;
		}
	}
	ret.collide = false;
	return ret;
}

void CollidesControler::update()
{
	for (int i = 0; i < dynamic_obbs.size(); i++) {
		for (int i2 = 0; i2 < static_obbs.size(); i2++) {
			if ((*dynamic_obbs[i].obb).is_collide(*static_obbs[i2]) || (*static_obbs[i2]).is_collide(*dynamic_obbs[i].obb))
				dynamic_obbs[i].answr.collide = true;
		}
	}
}
