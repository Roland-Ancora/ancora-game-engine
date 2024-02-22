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

		float near_plane = 1.0f, far_plane = 70.5f;
		glm::vec3 global_light_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 global_light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 global_light_view = glm::lookAt(global_light_pos,
													glm::vec3(0.0f, 0.0f, 0.0f),
													glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = global_light_proj * global_light_view;
	public:
		Foton();
		void use();
		void end();
		void set_global_light_positiob(glm::vec3 pos) {
			global_light_pos = pos;
			global_light_view = glm::lookAt(pos,
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = global_light_proj * global_light_view;
		}
	};

}
