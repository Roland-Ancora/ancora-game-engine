#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../depends/glm/glm.hpp>
#include <../depends/glm/gtc/matrix_transform.hpp>
#include "ageShaderProgram.h"
#include "ageCamera.h"
#include "ageWindow.h"
#include "ageCONSTANTS.h"



namespace age {

	class Foton {
		unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		ShaderProgram depth_sg = ShaderProgram::create_shader_program(AGE_DEFAULT_3D_DEPTH_MAP_VERTEX_SHADER, AGE_DEFAULT_3D_DEPTH_MAP_FRAGMENT_SHADER);
		GLuint light_space_matrix_loc;
		GLuint default_fogging_loc;
		GLuint shadow_intensity_loc;
		GLuint global_light_color_loc;
		GLuint global_light_minimal_color_loc;

		float near_plane = 1.0f, far_plane = 70.5f;
		float ortho_width_x = 20.0f, ortho_width_y = 20.0f;
		glm::vec3 global_light_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 global_light_proj = glm::ortho(-ortho_width_x / 2, ortho_width_x / 2, -ortho_width_y / 2, ortho_width_y / 2, near_plane, far_plane);
		glm::mat4 global_light_view = glm::lookAt(global_light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 global_light_center = glm::vec3(-1.0f, -1.0f, -1.0f);
		glm::mat4 lightSpaceMatrix = global_light_proj * global_light_view;
		float default_fogging = 1.0f;
		float shadow_intensity = 0.4f;
		glm::vec3 global_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 global_light_minimal_color = glm::vec3(0.25f, 0.25f, 0.25f);
	public:
		Foton();
	public:
		float get_default_fogging() { return default_fogging; }
		float get_shadow_intensity() { return shadow_intensity; }
		glm::vec3 get_global_light_color() { return global_light_color; }
		glm::vec3 get_global_light_minimal_color() { return global_light_minimal_color; }
		float get_global_light_field_width_x() { return ortho_width_x; }
		float get_global_light_field_width_y() { return ortho_width_y; }
		unsigned int get_global_light_resolution() { return SHADOW_WIDTH; }
	public:
		void set_default_fogging(float fogging);
		void set_shadow_intensity(float intensity);
		void set_global_light_color(glm::vec3 color);
		void set_global_light_minimal_color(glm::vec3 color);
		void use();
		void end();
		void set_global_light_position(glm::vec3 pos);
		void set_global_light_center(glm::vec3 center);
		void set_global_light_field_size(float width_x, float width_y);
		void set_global_light_resolution(unsigned int res);
	};

}
