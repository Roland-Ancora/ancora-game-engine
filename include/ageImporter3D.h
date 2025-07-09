//#################################################################//
//
//							Importer3D
//	
//		The Importer3D class is responsible for loading 3D model 
//	data.
// 
//		The Importer3D class has external dependencies: ASSIMP, 
//	GLM.
//		The Importer3D class has dependencies: ageTexture2D,
//	ageModel3dData, ageAnimation3D.
// 
// 
//		
//		NOTES ----
//			load_model requires directory name and file name apart.
//	But it should require only one full file name.
//			load_animation loads animation only from first channel.
//
//
//#################################################################//



#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ageTexture2D.h"
#include "ageModel3dData.h"
#include "ageAnimation3D.h"



namespace age {

	enum AGE_IMPORT_MODELS_GROUP_TYPE { AGE_IMGT_DCMPS_MATRIX, AGE_IMGT_N_DCMPS_MATRIX };

	class Importer3D {
		Importer3D();

		static Assimp::Importer importer;
		static glm::mat4 mat4_ai_to_glm(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }

		static std::string get_diffuse_texture_file_name(aiMesh* mesh, const aiScene* scene, std::string self_dir, std::string self_file_name);
		static void get_vertices_data_from_mesh(aiMesh* mesh, Model3dData& model);
		static void get_texture_coords_data_from_mesh(aiMesh* mesh, Model3dData& model);
		static void get_normals_data_from_mesh(aiMesh* mesh, Model3dData& model);
		static void get_indices_data_from_mesh(aiMesh* mesh, Model3dData& model);
		static void load_model_group_node(aiNode* node, const aiScene* scene, std::string dir_name, std::string file_name, Model3dGroupDataNode& model_data_node, AGE_IMPORT_MODELS_GROUP_TYPE type);
	public:
		static int load_model(std::string dir_name, std::string file_name, Model3dData& model_data);
		static int load_model_group(std::string dir_name, std::string file_name, Model3dGroupData& model_group_data, AGE_IMPORT_MODELS_GROUP_TYPE type = AGE_IMGT_N_DCMPS_MATRIX);
		static int load_animation(std::string dir_name, std::string file_name, Animation3D& animation_data);
	};

}
