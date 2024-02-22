#pragma once



namespace age {

	const int AGE_WINDOW_OBJECT_OVERLAPPING_ERROR = 11;
	const int AGE_CAMERA_OBJECT_OVERLAPPING_ERROR = 12;

	const int AGE_SHADER_PROGRAM_MAX_LENGHT = 5000;
	const int AGE_SHADER_OPEN_ERROR = 21;

	const char AGE_DEFAULT_3D_VERTEX_SHADER[] = "./resources/shaders/age_main_3d_shader.vert";
	const char AGE_DEFAULT_3D_FRAGMENT_SHADER[] = "./resources/shaders/age_main_3d_shader.frag";
	const char AGE_DEFAULT_2D_VERTEX_SHADER[] = "./resources/shaders/age_main_2d_shader.vert";
	const char AGE_DEFAULT_2D_FRAGMENT_SHADER[] = "./resources/shaders/age_main_2d_shader.frag";
	const char AGE_DEFAULT_3D_DEPTH_MAP_VERTEX_SHADER[] = "./resources/shaders/age_main_3d_depth_map_shader.vert";
	const char AGE_DEFAULT_3D_DEPTH_MAP_FRAGMENT_SHADER[] = "./resources/shaders/age_main_3d_depth_map_shader.frag";

}
