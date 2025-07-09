//#################################################################//
//
//								Model3dData
//	
//		The Model3dData structure is a structure for storing simple
//	data about a 3D model.
//		Why Model3dData haven't function for loading data from
//	file? First reason: Assimp, that use for loading data has one
//	Assimp::Importer for all loads. If it will be here like a
//	static  field, this would be wrong in terms of project
//	architecture.
//
//		This module has external dependencies: GLM.
//		The Model3dData structure has dependencies: ageTexture2D.
//
//
//#################################################################//



#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ageTexture2D.h"



namespace age {

	struct Model3dData {
		~Model3dData();
	
		Texture2D diffuse_texture;
		float* vertices;
		float* texture_coords;
		float* normals;
		unsigned int* indices;
		unsigned int vertices_cnt = 0, texture_coords_cnt = 0, normals_cnt = 0, indices_cnt = 0;

		void clear();
	};

	struct Model3dGroupDataNode {
		~Model3dGroupDataNode();

		glm::vec3 position, rotation, scale;
		glm::quat rotation_quat;
		glm::mat4 ALTR_fin_model_mat = glm::mat4(0);
		Model3dData model;
		Model3dGroupDataNode* childs;
		int childs_count = 0;

		void clear();
	};

	struct Model3dGroupData {
		~Model3dGroupData();

		Model3dGroupDataNode* childs;
		int childs_count = 0;

		void clear();
	};

}