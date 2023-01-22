//#############################################################################//
//
//									Texture2D
// 
//		The Texture2D module contains Texture2D class. This class recuire
//	for a loading and contain 2D texture (image) from the image file. This
//	module uses stb_image.h, and supports all image files thats stb support.
// 
//		The Camera class has external dependencies: STB, OpenGL.
// 
//		Also, Texture2D has operator GLuint, he returns OpenGL texture ID
//	for a backward compatibillity.
//		During loading, some OpenGL parameters are also set: GL_TEXTURE_WRAP_S,
//	GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER.
//
//
//#############################################################################//



#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"



namespace age {

	class Texture2D {
		GLuint texture_id;
		int texture_width, texture_height;
	public:
		~Texture2D();
		int load_from_file(const char* file_name);
		int get_width() { return texture_width; }
		int get_height() { return texture_height; }
		operator GLuint () const { return texture_id; }
	};

}
