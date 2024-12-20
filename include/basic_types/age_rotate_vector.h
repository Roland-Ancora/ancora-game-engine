//#################################################################//
// 
//							rotate_vector
// 
//		This module contains type for defenition rotation axis for
//	scene objects.
// 
//		The Font module has external dependencies: GLM.
// 
// 
//#################################################################//



#pragma once
#include "glm/glm.hpp"



namespace age {

	typedef glm::vec3 rotate_vector;
	const rotate_vector AGE_ROTATE_AROUND_X = glm::vec3(1, 0, 0);
	const rotate_vector AGE_ROTATE_AROUND_Y = glm::vec3(0, 1, 0);
	const rotate_vector AGE_ROTATE_AROUND_Z = glm::vec3(0, 0, 1);

}
