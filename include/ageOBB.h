//#################################################################//
// 
//								OBB
// 
//		OBB module contains box collides structures and mechanisms.
// 
//		The Font module has external dependencies: GLM.
//		The Font module has dependencies: Model3D, Model3dData.
// 
// 
//#################################################################//



#pragma once
#include <glm/glm.hpp>
#include "ageModel3dData.h" 
#include "ageModel3D.h"



namespace age {

	enum obb_type { OBB_OBB, OBB_MODEL, OBB_MODEL_GROUP };
	enum obb_sides { OBB_SIDE_ULF, OBB_SIDE_URF, OBB_SIDE_DRF, OBB_SIDE_DLF,
					OBB_SIDE_ULB, OBB_SIDE_URB, OBB_SIDE_DRB, OBB_SIDE_DLB };

	struct OBB {
		glm::vec3 points[8];
		glm::mat4 o_mat = glm::mat4(1), r_mat = glm::mat4(1);
	public:
		static void set_from_model_data(OBB& obb, Model3dData* model_data);
		virtual bool is_collide(OBB obb);
		virtual bool is_collide(glm::vec3 point);
	public:
		void set_points(glm::vec3 s_point, glm::vec3 sides);
	};

	struct OBB_Model : public OBB {
		Model3D* model;
	public:
		static void set_from_model(OBB_Model& obb, Model3D* model3d);
	public:
		void update_matrices();
	};

	struct OBB_Model_Group : public OBB_Model {
		OBB_Model* obbs;
		int obbs_cnt = 0;
	public:
		~OBB_Model_Group() { delete[] obbs; }
		static void set_from_model_group(OBB_Model_Group& obb, Model3dGroup* model_group);
		virtual bool is_collide(OBB obb);
		virtual bool is_collide(glm::vec3 point);
	public:
		void update_matrices();
	};

}