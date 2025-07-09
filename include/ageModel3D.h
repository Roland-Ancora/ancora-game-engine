//#################################################################//
//
//				Model3D, Model3dGroup, Model3dGroupNode
//	
//		The Model3D class is responsible for showing 3D model.
//	Model3dGroup class is responsible for showing more 3D models.
// 
//		This module has external dependencies: OpenGL, GLM, STD.
//		This module has dependencies: ageModel3dData, ageCamera, 
//	ageAnimation3D, ageProgramClock, age_rotate_vector.
// 
//		Why Model3dGroupNode is friend class for Model3dGroup? For
//	a now, Model3dGroupNode haven't any methods to interactive with
//	user. Also, not logic if Model3dGroup will be parent class for
//	Model3dGroupNode. It turns out that Model3dGroupNode must be
//	private class, and must be available for Model3dGroup. 
//	Therefore, the mechanism of friendly classes is used.
// 
// 
// 
//			NOTES
//		The opengl_draw method finding uniform locations every
//	frame.
//
//
//#################################################################//



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "ageModel3dData.h"
#include "ageCamera.h"
#include "ageAnimation3D.h"
#include "ageProgramClock.h"
#include "basic_types/age_rotate_vector.h"



namespace age {

	class Model3D {
	protected:
		Model3dData* model_data = 0;
		glm::mat4 ALTR_fin_model_mat = glm::mat4(0);
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
		float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;
		float x_rot = 0.0f, y_rot = 0.0f, z_rot = 0.0f;
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float color_intensity = 0.0f;
		GLuint vboIDs[4];
		GLuint vaoID;
	protected:
		void opengl_draw();
	public:
		Model3D() { glGenBuffers(4, vboIDs); glGenVertexArrays(1, &vaoID); }
		Model3D(Model3dData* data) { Model3D(); set_model_3d_data(data); }
		virtual void show();
	public:
		glm::vec3 get_position() { return glm::vec3(x_pos, y_pos, z_pos); }
		glm::vec3 get_scale() { return glm::vec3(x_scale, y_scale, z_scale); }
		glm::mat4 get_model_matrix() { return ALTR_fin_model_mat == glm::mat4(0) ? translate_mat * rotate_mat * scale_mat : ALTR_fin_model_mat; }
		glm::vec3 get_color() { return color; }
		Model3dData* get_model_data() { return model_data; }
		void set_color(float r, float g, float b) { color = glm::vec3(r, g, b); }
		float get_color_intensity() { return color_intensity; }
		void set_color_intensity(float intens) { color_intensity = intens; }
		glm::mat4 get_rotation_matrix() { return rotate_mat; }
	public:
		void set_model_3d_data(Model3dData* model_3d_data);
		void set_position(float x, float y, float z);
		void move(float x, float y, float z);
		void set_rotation(float angle, rotate_vector vec);
		void set_rotation(glm::mat4 matrix);
		void set_only_rotation_mat(glm::mat4 matrix) { rotate_mat = matrix; }
		void set_alternative_finnaly_matrix(glm::mat4 matrix) { ALTR_fin_model_mat = matrix; }
		void rotate(float angle, rotate_vector vec);
		void set_scale(float x, float y, float z);
		glm::mat4 get_reverse_matrix();
	public:
		Model3D& operator = (const Model3D& model)
		{
			if (&model != this)
			{
				model_data = model.model_data;
				rotate_mat = model.rotate_mat, translate_mat = model.translate_mat, scale_mat = model.scale_mat;
				x_pos = model.x_pos, y_pos = model.y_pos, z_pos = model.z_pos;
				x_scale = model.x_scale, y_scale = model.y_scale, z_scale = model.z_scale;
				x_rot = model.x_rot, y_rot = model.y_rot, z_rot = model.z_rot;

				glGenBuffers(4, vboIDs); 
				glGenVertexArrays(1, &vaoID);
				if (model_data != 0) {
					glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
					glBufferData(GL_ARRAY_BUFFER, model_data->vertices_cnt * sizeof(float), model_data->vertices, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
					glBufferData(GL_ARRAY_BUFFER, model_data->texture_coords_cnt * sizeof(float), model_data->texture_coords, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, model_data->normals_cnt * sizeof(float), model_data->normals, GL_STATIC_DRAW);
				}
			}
			return *this;
		}
	};



	class Model3D_Anim__child;

	class Model3D_Anim : public Model3D {
	protected:
		Animation3D* anim;
		int anim_last_node = 0;
		glm::mat4 anim_matrix = glm::mat4(1);
		double now_play_time = 0.0f;
		std::vector<Model3D_Anim__child*> childs;
		bool is_anim_loop = false;
		bool anim_play = false;
		friend class Model3D_Anim__child;
	protected:
		void update_anim_matrix();
	public:
		Model3D_Anim() { }
		virtual void show();
	public:
		void set_animation(Animation3D* animation_data) { anim = animation_data; };
		void animation_play() { anim_play = true; };
		void animation_stop() { anim_play = false; };
		void set_animation_loop() { is_anim_loop = true; }
		void disable_animation_loop() { is_anim_loop = false; }
		double get_anim_progress() { return now_play_time / anim->anim_duration; }
		bool is_anim_play() { return anim_play; }
	public:
		void show_model_only();
	};
	
	class Model3D_Anim__child : public Model3D_Anim {
	public:
		void set_parent_object(Model3D_Anim* obj) { obj->childs.push_back(this); }
	public:
		void show_child(glm::mat4 parent_mat);
		void show_child_model_only(glm::mat4 parent_mat);
	};

	class Model3dGroupNode {
		Model3D model;
		Model3dGroupNode* childs;
		int childs_count = 0;
		friend class Model3dGroup;
	private:
		void set_from_data_node(Model3dGroupDataNode* node);
		void show();
	public:
		~Model3dGroupNode();
	public:
		int get_childs_count() { return childs_count; }
		Model3dGroupNode* get_child(int id) { return &childs[id]; }
		Model3D* get_model() { return &model; }
	};

	class Model3dGroup {
		Model3dGroupNode* childs;
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4 model_matrix = glm::mat4(1);
		int childs_count = 0;
	public:
		~Model3dGroup();
		Model3dGroup() {}
		Model3dGroup(Model3dGroupData* data) { set_group_from_data(data); }
	public:
		int get_childs_count() { return childs_count; }
		Model3dGroupNode* get_child(int id) { return &childs[id]; }
		glm::vec3 get_position() { return position; }
		glm::mat4 get_model_matrix() { return model_matrix; }
	public:
		void set_group_from_data(Model3dGroupData* data);
		void set_position(glm::vec3 pos);
		void set_scale(glm::vec3 scl);
		void show();
		void clear();
	};

}
