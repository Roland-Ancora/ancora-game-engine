#pragma once
#include <../depends/glm/glm.hpp>

#include "ageTexture2D.h"
#include "ageCamera.h"



namespace age {

	class Sprite2D {
		Texture2D* texture = 0;
		float pos_x = 0, pos_y = 0;
		glm::mat4 model_mat = glm::mat4(1);
		glm::mat4 translate_mat = glm::mat4(1), scale_mat = glm::mat4(1), rotate_mat = glm::mat4(1);
		float aspect_ratio = 1.0f; // texture height to width ratio
		GLuint vboIDs[2];
		GLuint vaoID;

		static float vert_pos_data[8];
		static float tex_pos_data[8];
	public:
		Sprite2D();

		void set_texture(Texture2D* tex);
		void show();
		void move(float x, float y);
		void move_x(float x);
		void move_y(float y);
	};

}
