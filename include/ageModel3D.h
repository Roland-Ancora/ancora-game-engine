//########################################################################//
//
//					Model3D, Model3dGroup, Model3dGroupNode
//	
//		The Model3D class is responsible for showing 3D model.
//	Model3dGroup class is responsible for showing more 3D models.
// 
//		This module has external dependencies: OpenGL, GLM.
//		This module has dependencies: ageModel3dData, Camera.
// 
//		Why Model3dGroupNode is friend class for Model3dGroup? For a now,
//	Model3dGroupNode haven't any methods to interactive with user. Also,
//	not logic if Model3dGroup will be parent class for Model3dGroupNode.
//	It turns out that Model3dGroupNode must be private class, and must
//	be available for Model3dGroup. Therefore, the mechanism of friendly
//	classes is used.
//
//
//########################################################################//



#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../depends/glm/glm.hpp"
#include "../depends/glm/gtc/matrix_transform.hpp"
#include "../include/ageModel3dData.h"
#include "../include/ageCamera.h"



namespace age {

	typedef glm::vec3 rotate_vector;
	const rotate_vector AGE_ROTATE_AROUND_X = glm::vec3(1, 0, 0);
	const rotate_vector AGE_ROTATE_AROUND_Y = glm::vec3(0, 1, 0);
	const rotate_vector AGE_ROTATE_AROUND_Z = glm::vec3(0, 0, 1);



	class Model3D {
		Model3dData* model_data;
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
	public:
		void set_model_3d_data(Model3dData* model_3d_data) { model_data = model_3d_data; }
		void show();
		void set_position(float x, float y, float z);
		void move(float x, float y, float z);
		glm::vec3 get_position() { return glm::vec3(x_pos, y_pos, z_pos); }
		void set_rotation(float angle , rotate_vector vec);
		void rotate(float angle, rotate_vector vec);
	};

	class Model3dGroupNode {
		Model3D model;
		Model3dGroupNode* childs;
		int childs_count = 0;

		friend class Model3dGroup;

		void set_from_data_node(Model3dGroupDataNode* node);
		void show();
	public:
		~Model3dGroupNode();
	};

	class Model3dGroup {
		Model3dGroupNode* childs;
		int childs_count = 0;
	public:
		~Model3dGroup();
		void set_group_from_data(Model3dGroupData* data);
		void show();
	};

}
