#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ageCamera.h"
#include "ageCONSTANTS.h"



namespace age {

	class Skybox {
		GLuint texture_id;
		static ShaderProgram skybox_shader;
		static GLuint skybox_shader_texture_loc;
		static GLuint vao, vbo;
		static glm::vec3 skybox_verticies[];
	public:
		Skybox();
		Skybox(const char* file_names[6]);
		void load_from_files(const char* file_names[6]);
		void show();
	};

}
