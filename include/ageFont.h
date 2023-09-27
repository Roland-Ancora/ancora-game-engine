#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <map>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "ageShaderProgram.h"
#include "../depends/glm/glm.hpp"




namespace age {

	struct Character {
		GLuint TextureID; // glyph texture ID
		glm::vec2 Size;      // glyph size
		glm::vec2 Bearing;   // �������� ������� ����� ����� �����
		float Advance;   // �������������� �������� �� ������ ���������� �����
	};




	class Font {
		FT_Library library;
		FT_Face font_face;
	public:
        std::map<GLchar, Character> Characters;
        ShaderProgram text_sh_program;
		Font(const char* file_name);
		Font() {}
		void load_font(const char* file_name);
	};

}
