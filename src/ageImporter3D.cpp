#include "../include/ageImporter3D.h"



using namespace age;

Assimp::Importer Importer3D::importer;



// Returns diffuse texture file name fomr mesh. Returns empty string if mesh hasn't diffuse texture.
std::string Importer3D::get_diffuse_texture_file_name(aiMesh* mesh, const aiScene* scene, std::string self_dir, std::string self_file_name)
{
	if (mesh->mMaterialIndex < 0)
		return "";
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0)
		return "";
	aiString texture_file_name;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_file_name);
	std::string texture_file_name_C_Str = texture_file_name.C_Str();
	if (texture_file_name_C_Str[0] == '/' || texture_file_name_C_Str[1] == ':')
		return texture_file_name_C_Str;
	return self_dir + texture_file_name_C_Str;
}

// Load vertices from mesh in model data.
void Importer3D::get_vertices_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model)
{
	model.vertices_cnt = mesh->mNumVertices * 3;
	model.vertices = new float[model.vertices_cnt];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		model.vertices[i * 3] = mesh->mVertices[i].x;
		model.vertices[i * 3 + 1] = mesh->mVertices[i].y;
		model.vertices[i * 3 + 2] = mesh->mVertices[i].z;
	}
}

// Load texture coords from mesh in model data.
void Importer3D::get_texture_coords_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model)
{
	model.texture_coords_cnt = mesh->mNumVertices * 2;
	model.texture_coords = new float[model.texture_coords_cnt];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		model.texture_coords[i * 2] = mesh->mTextureCoords[0][i].x;
		model.texture_coords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
	}
}

// Load normals from mesh in model data.
void Importer3D::get_normals_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model)
{
	model.normals_cnt = mesh->mNumVertices * 3;
	model.normals = new float[model.normals_cnt];
	for (int i = 0; i < mesh->mNumVertices; i++) {
		model.normals[i * 3] = mesh->mNormals[i].x;
		model.normals[i * 3 + 1] = mesh->mNormals[i].y;
		model.normals[i * 3 + 2] = mesh->mNormals[i].z;
	}
}

// Load indices from mesh in model data.
void Importer3D::get_indices_data_from_mesh(aiMesh* mesh, const aiScene* scene, Model3dData& model)
{
	model.indices_cnt = mesh->mNumFaces * 3;
	model.indices = new unsigned int[model.indices_cnt];
	for (int i = 0; i < mesh->mNumFaces; i++) {
		model.indices[i * 3] = mesh->mFaces[i].mIndices[0];
		model.indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		model.indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
}

// This function load first model in file in Model3dData structure.
int Importer3D::load_model(std::string dir_name, std::string file_name, Model3dData& model_data)
{
	const aiScene* scene = importer.ReadFile(dir_name + file_name, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("Error: Can't load model. %s\n", importer.GetErrorString());
		return -1;
	}
	aiNode* first_child_node = scene->mRootNode->mChildren[0];
	aiMesh* the_mesh = scene->mMeshes[first_child_node->mMeshes[0]];
	get_vertices_data_from_mesh(the_mesh, scene, model_data);
	get_texture_coords_data_from_mesh(the_mesh, scene, model_data);
	get_normals_data_from_mesh(the_mesh, scene, model_data);
	get_indices_data_from_mesh(the_mesh, scene, model_data);
	std::string texture_file = get_diffuse_texture_file_name(the_mesh, scene, dir_name, file_name);
	if (texture_file != "")
		model_data.diffuse_texture.load_from_file(texture_file.c_str());
	importer.FreeScene();
	return 1;
}

void Importer3D::load_model_group_node(aiNode* node, const aiScene* scene, std::string dir_name, std::string file_name, Model3dGroupDataNode& model_data_node)
{
	if (node->mNumMeshes > 0) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
		get_vertices_data_from_mesh(mesh, scene, model_data_node.model);
		get_texture_coords_data_from_mesh(mesh, scene, model_data_node.model);
		get_normals_data_from_mesh(mesh, scene, model_data_node.model);
		get_indices_data_from_mesh(mesh, scene, model_data_node.model);
		std::string texture_file = get_diffuse_texture_file_name(mesh, scene, dir_name, file_name);
		if (texture_file != "")
			model_data_node.model.diffuse_texture.load_from_file(texture_file.c_str());
	}

	aiVector3D pos, rot, scale;
	node->mTransformation.Decompose(scale, rot, pos);
	model_data_node.position = glm::vec3(pos.x, pos.y, pos.z);
	model_data_node.rotation = glm::vec3(rot.x, rot.y, rot.z);
	model_data_node.scale = glm::vec3(scale.x, scale.y, scale.z);

	model_data_node.childs_count = node->mNumChildren;
	model_data_node.childs = new Model3dGroupDataNode[model_data_node.childs_count];
	for (int i = 0; i < model_data_node.childs_count; i++)
		load_model_group_node(node->mChildren[i], scene, dir_name, file_name, model_data_node.childs[i]);
}

int Importer3D::load_model_group(std::string dir_name, std::string file_name, Model3dGroupData& model_group_data)
{
	const aiScene* scene = importer.ReadFile(dir_name + file_name, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("Error: Can't load model. %s\n", importer.GetErrorString());
		return -1;
	}
	aiNode* root_node = scene->mRootNode;
	model_group_data.childs_count = root_node->mNumChildren;
	model_group_data.childs = new Model3dGroupDataNode[model_group_data.childs_count];
	for (int i = 0; i < model_group_data.childs_count; i++)
		load_model_group_node(root_node->mChildren[i], scene, dir_name, file_name, model_group_data.childs[i]);
	importer.FreeScene();
	return 1;
}
