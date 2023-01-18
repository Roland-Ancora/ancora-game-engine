//########################################################################//
//
//							ShaderProgram
//	
//		The ShaderProgram class is responsible for the creating shaders
//	with static functions. Stores the ID and location of the main MV and 
//	MVP matrix uniform variables of the created shader program.
//
//		The ShaderProgram class has external dependencies: GLFW, OpenGL,
// std.
// 
// 
// 
// 
//		NOTE ---
//			ShaderProgram must have dynamic loading of uniform variables,
//	may be in any dinamyc structure.
//			ShaderProgram can create a shader only from vertex and fragment
//	shader files.
//			shader_max_len is 5000 symbols.
//
//
//########################################################################//



#pragma once
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"



namespace age {

	class ShaderProgram
	{
		GLuint shader_prog_id, MVP_mat_loc, MV_mat_loc;
	public:
		GLuint get_shader_program_id() { return shader_prog_id; }
		GLuint get_MVP_matrix_location() { return MVP_mat_loc; }
		GLuint get_MV_matrix_location() { return MV_mat_loc; }

		static ShaderProgram create_main_shader_3d_program();
		static ShaderProgram create_main_shader_2d_program();
		static ShaderProgram create_shader_program(const char* vert_shader_file, const char* frag_shader_file);
	};

}
