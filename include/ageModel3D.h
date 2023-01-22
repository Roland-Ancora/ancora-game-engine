//#####################################################################//
//
//								Model3D
//	
//		The Model3D class is responsible for showing 3D model.
// 
//		This module has external dependencies: OpenGL, GLM.
//		This module has dependencies: ageModel3dData, Camera.
//
//
//#####################################################################//



#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../depends/glm/glm.hpp"
#include "../depends/glm/gtc/matrix_transform.hpp"
#include "../include/ageModel3dData.h"
#include "../include/ageCamera.h"



namespace age {
	class Model3D {
		Model3dData* model_data;
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		glm::mat4 model_matrix = glm::mat4(1);
	public:
		void set_model_3d_data(Model3dData* model_3d_data) { model_data = model_3d_data; }
		void show();
	};
}
