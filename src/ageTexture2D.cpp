#include "../include/ageTexture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../depends/stb_image.h"



using namespace age;



Texture2D::~Texture2D()
{
	glDeleteTextures(1, &texture_id);
}

// Load image texture from file_name. Returns -1 if texture didn't load and 1 if it is.
int Texture2D::load_from_file(const char* file_name)
{
	int tex_width, tex_height, tex_channels;
	GLubyte* img = stbi_load(file_name, &tex_width, &tex_height, &tex_channels, 0);
	if (img == NULL) {
		printf("Error: %s image was not load!\n", file_name);
		return -1;
	}
	texture_width = tex_width;
	texture_height = tex_height;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
		0, tex_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img);
	return 1;
}

int Texture2D::load_from_file(const char* file_name, texture_filter filter)
{
	int tex_width, tex_height, tex_channels;
	GLubyte* img = stbi_load(file_name, &tex_width, &tex_height, &tex_channels, 0);
	if (img == NULL) {
		printf("Error: %s image was not load!\n", file_name);
		return -1;
	}
	texture_width = tex_width;
	texture_height = tex_height;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
		0, tex_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img);
	return 1;
}
