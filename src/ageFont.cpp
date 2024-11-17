#include "ageFont.h"



using namespace age;



ShaderProgram Font::font_sh_prog;

void Font::Init()
{
    font_sh_prog = ShaderProgram::create_shader_program(AGE_DEFAULT_UI_TEXT_VERTEX_SHADER, AGE_DEFAULT_UI_TEXT_FRAGMENT_SHADER);
}

Font::Font(const char* file_name)
{
    load_font(file_name);
}

void Font::load_font(const char* file_name)
{
    float font_size = 148.0f;
    if (FT_Init_FreeType(&library))
        printf("AGE::ERROR::FONT:: Error with creating FreeType initialize\n");
    if (FT_New_Face(library, file_name, 0, &font_face))
        printf("AGE::ERROR::FONT:: Error with load font\n");
    FT_Set_Pixel_Sizes(font_face, 0, static_cast<FT_UInt>(font_size));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(font_face, c, FT_LOAD_RENDER))
        {
            std::cout << "AGE::ERROR::FONT:: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            font_face->glyph->bitmap.width,
            font_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            font_face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::vec2(font_face->glyph->bitmap.width / font_size, font_face->glyph->bitmap.rows / font_size),
            glm::vec2(font_face->glyph->bitmap_left / font_size, font_face->glyph->bitmap_top / font_size),
            (font_face->glyph->advance.x >> 6) / font_size
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
}
