//#################################################################//
// 
//								Font
// 
//		Font module is contain class Font and struct Character for
//	loading and store fonts. Uses FreeType library and supports
//	all font formats that supports FreeType.
// 
//		The Font module has external dependencies: FreeType, STD,
//	OpenGL, GLM.
//		The Font module has dependencies: ShaderProgram.
// 
// 
//#################################################################//



#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ageShaderProgram.h"
#include "ageCONSTANTS.h"



namespace age {

	struct Character {
		GLuint TextureID; // glyph texture ID
		glm::vec2 Size;      // glyph size
		glm::vec2 Bearing;   // Смещение верхней левой точки глифа
		float Advance;   // Горизонтальное смещение до начала следующего глифа
	};



	class Font {
		FT_Library library;
		FT_Face font_face;
		static ShaderProgram font_sh_prog;
	public:
		std::map<GLchar, Character> Characters;
	public:
		Font(const char* file_name);
		Font() {}
		static void Init();
		static ShaderProgram* get_shader_program() { return &font_sh_prog; }
	public:
		void load_font(const char* file_name);
	};

}
