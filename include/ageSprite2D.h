//#################################################################//
//
//								Sprite2D
//	
//		The Sprite2D class is responsible for displaying sprites on
//	2D scene.
// 
//		The Sprite2D class has external dependencies: GLM.
//		The Sprite2D class has dependencies: ageTexture2D, 
//	ageCamera.
// 
//
//#################################################################//



#pragma once
#include <glm/glm.hpp>
#include "ageTexture2D.h"
#include "ageCamera.h"



namespace age {

	class Sprite2D {
		Texture2D* texture = 0;
		float pos_x = 0, pos_y = 0;
		glm::mat4 model_mat = glm::mat4(1);
		glm::mat4 translate_mat = glm::mat4(1), scale_mat = glm::mat4(1), rotate_mat = glm::mat4(1);
		float aspect_ratio = 1.0f; // texture height to width ratio
		GLuint vboID;
		GLuint vaoID;
	private:
		static float vert_tex_pos_data[16];
	public:
		Sprite2D();
	public:
		void set_texture(Texture2D* tex);
		void show();
		void move(float x, float y);
		void move_x(float x);
		void move_y(float y);
	};

}
