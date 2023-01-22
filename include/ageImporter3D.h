//#####################################################################//
//
//							Importer3D
//	
//		The Importer3D class is responsible for loading 3D model data.
// 
//		The Importer3D class has external dependencies: ASSIMP, GLM.
// 
//		The Importer3D class has dependencies: ageTexture2D,
//	ageModel3dData.
// 
// 
// 
//		
//		NOTES ----
//			load_model requires directory name and file name apart. But it
//	 should require only one full file name.
//
//
//#####################################################################//



#pragma once
#include "../depends/glm/glm.hpp"
#include "../depends/glm/gtc/matrix_transform.hpp"
#include "../depends/glm/gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ageTexture2D.h"
#include "ageModel3dData.h"


namespace age {

	class Importer3D {
		Importer3D();

		static Assimp::Importer importer;

		static std::string get_diffuse_texture_file_name(aiMesh* mesh, const aiScene* scene, std::string self_dir, std::string self_file_name);
		static void get_vertices_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model);
		static void get_texture_coords_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model);
		static void get_normals_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model);
		static void get_indices_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model);
	public:
		static int load_model(std::string dir_name, std::string file_name, Model3dData& model_data);
	};

}
