//########################################################################//
//
//					Model3D, Model3dGroup, Model3dGroupNode
//	
//		The Model3D class is responsible for showing 3D model.
//	Model3dGroup class is responsible for showing more 3D models.
// 
//		This module has external dependencies: OpenGL, GLM, STD.
//		This module has dependencies: ageModel3dData, ageCamera, 
//	ageAnimation3D, ageProgramClock, age_rotate_vector.
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



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../depends/glm/glm.hpp>
#include <../depends/glm/gtc/matrix_transform.hpp>
#include <vector>

#include "ageModel3dData.h"
#include "ageCamera.h"
#include "ageAnimation3D.h"
#include "ageProgramClock.h"
#include "basic_types/age_rotate_vector.h"



namespace age {

	class Model3D {
	protected:
		Model3dData* model_data;
		glm::mat4 rotate_mat = glm::mat4(1), translate_mat = glm::mat4(1), scale_mat = glm::mat4(1);
		float x_pos = 0.0f, y_pos = 0.0f, z_pos = 0.0f;
		float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;
		GLuint vboIDs[4];
		GLuint vaoID;

		void opengl_draw();
	public:
		Model3D() { glGenBuffers(4, vboIDs); glGenVertexArrays(1, &vaoID); }
		glm::vec3 get_position() { return glm::vec3(x_pos, y_pos, z_pos); }
		glm::vec3 get_scale() { return glm::vec3(x_scale, y_scale, z_scale); }
		virtual void show();
		void set_model_3d_data(Model3dData* model_3d_data);
		void set_position(float x, float y, float z);
		void move(float x, float y, float z);
		void set_rotation(float angle , rotate_vector vec);
		void rotate(float angle, rotate_vector vec);
		void set_scale(float x, float y, float z);
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
		void update_anim_matrix();
	public:
		Model3D_Anim() { }
		void set_animation(Animation3D* animation_data) { anim = animation_data; };
		void animation_play() { anim_play = true; };
		void animation_stop() { anim_play = false; };
		void set_animation_loop() { is_anim_loop = true; }
		void disable_animation_loop() { is_anim_loop = false; }
		float get_anim_progress() { return now_play_time / anim->anim_duration; }
		bool is_anim_play() { return anim_play; }
		virtual void show();
		void show_model_only();
	};
	
	class Model3D_Anim__child : public Model3D_Anim {
	public:
		void set_parent_object(Model3D_Anim* obj) { obj->childs.push_back(this); }
		void show_child(glm::mat4 parent_mat);
		void show_child_model_only(glm::mat4 parent_mat);
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
