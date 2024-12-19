#pragma once
#include <vector>
#include "ageCONSTANTS.h"
#include "ageOBB.h"
#include "additions/ageForwardList.hpp"



namespace age {

	enum obb_controler_element_type { OBB_DYNAMIC, OBB_STATIC };

	struct collide_obb_answer {
		bool collide = false;
	};

	struct dynamic_obb_struct {
		OBB* obb;
		collide_obb_answer answr;
	public:
		dynamic_obb_struct() { }
		dynamic_obb_struct(OBB* Obb) : obb(Obb) {}
	};

	class CollidesControler {
		std::vector<dynamic_obb_struct> dynamic_obbs;
		std::vector<OBB*> static_obbs;
		static CollidesControler* active_singltone;
	private:
		void add_obb_by_type(std::vector<OBB*>* vect, obb_type type);
	public:
		CollidesControler();
		static CollidesControler* get_singltone() { return active_singltone; }
	public:
		collide_obb_answer get_answer(int id) { return dynamic_obbs[id].answr; }
		void clear_dynamic_obbs() { dynamic_obbs.clear(); }
	public:
		int add_obb(OBB* obb, obb_controler_element_type type_el, obb_type type_obb);
		collide_obb_answer test_obb_with_statics(OBB* obb);
		void update();
	};

}
