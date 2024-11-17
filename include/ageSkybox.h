//#################################################################//
//
//								Skybox
//	
//		The Skybox class is responsible for displaying skybox on 3D
//	scene.
// 
//		The Skybox class has external dependencies: GLFW, OpenGL.
//		The Skybox class has dependencies: ageCamera.
//
//
//#################################################################//



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ageCamera.h"
#include "ageCONSTANTS.h"



namespace age {

	class Skybox {
		GLuint texture_id;
	private:
		static ShaderProgram skybox_shader;
		static GLuint skybox_shader_texture_loc;
		static GLuint vao, vbo;
		static glm::vec3 skybox_verticies[];
	public:
		Skybox();
		Skybox(const char* file_names[6]);
	public:
		void load_from_files(const char* file_names[6]);
		void show();
	};

}
