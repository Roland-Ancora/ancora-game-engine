#pragma once



namespace age {

	const int AGE_WINDOW_OBJECT_OVERLAPPING_ERROR = 11;
	const int AGE_CAMERA_OBJECT_OVERLAPPING_ERROR = 12;
	const int AGE_AXISLINES_OBJECT_OVERLAPPING_ERROR = 13;
	const int AGE_UI_WINDOW_OBJECT_OVERLAPPING_ERROR = 14;

	const int AGE_SHADER_PROGRAM_MAX_LENGHT = 5000;
	const int AGE_SHADER_OPEN_ERROR = 21;

	const char AGE_DEFAULT_3D_VERTEX_SHADER[] = "./resources/shaders/age_main_3d_shader.vert";
	const char AGE_DEFAULT_3D_FRAGMENT_SHADER[] = "./resources/shaders/age_main_3d_shader.frag";
	const char AGE_DEFAULT_2D_VERTEX_SHADER[] = "./resources/shaders/age_main_2d_shader.vert";
	const char AGE_DEFAULT_2D_FRAGMENT_SHADER[] = "./resources/shaders/age_main_2d_shader.frag";
	const char AGE_DEFAULT_3D_DEPTH_MAP_VERTEX_SHADER[] = "./resources/shaders/age_main_3d_depth_map_shader.vert";
	const char AGE_DEFAULT_3D_DEPTH_MAP_FRAGMENT_SHADER[] = "./resources/shaders/age_main_3d_depth_map_shader.frag";
	const char AGE_DEFAULT_UI_VERTEX_SHADER[] = "./resources/shaders/age_main_ui_shader.vert";
	const char AGE_DEFAULT_UI_FRAGMENT_SHADER[] = "./resources/shaders/age_main_ui_shader.frag";
	const char AGE_DEFAULT_UI_FINNALY_TEXTURE_VERTEX_SHADER[] = "./resources/shaders/age_main_ui_fin_texture_shader.vert";
	const char AGE_DEFAULT_UI_FINNALY_TEXTURE_FRAGMENT_SHADER[] = "./resources/shaders/age_main_ui_fin_texture_shader.frag";
	const char AGE_DEFAULT_UI3D_VERTEX_SHADER[] = "./resources/shaders/age_main_ui3d_shader.vert";
	const char AGE_DEFAULT_UI3D_FRAGMENT_SHADER[] = "./resources/shaders/age_main_ui3d_shader.frag";
	const char AGE_DEFAULT_UI_TEXT_VERTEX_SHADER[] = "resources/shaders/age_main_ui_text_shader.vert";
	const char AGE_DEFAULT_UI_TEXT_FRAGMENT_SHADER[] = "resources/shaders/age_main_ui_text_shader.frag";
	const char AGE_AXISLINES_VERTEX_SHADER[] = "resources/shaders/additions/age_axislines_shader.vert";
	const char AGE_AXISLINES_FRAGMENT_SHADER[] = "resources/shaders/additions/age_axislines_shader.frag";
	const char AGE_DEFAULT_3D_SKYBOX_VERTEX_SHADER[] = "resources/shaders/age_main_3d_skybox_shader.vert";
	const char AGE_DEFAULT_3D_SKYBOX_FRAGMENT_SHADER[] = "resources/shaders/age_main_3d_skybox_shader.frag";

	const char AGE_UI3D_SHADER_UNIFORM_ALPHA_CHANNEL_NAME[] = "alpha_channel";

	const int AGE_UI_VIDEO_OPEN_FILE_ERROR = -1;
	const int AGE_UI_VIDEO_STREAM_NOT_FOUND = -2;
	const int AGE_UI_VIDEO_STREAM_NOT_FOUND_OR_UNSUPPORTED_CODEC = -3;
	const int AGE_UI_VIDEO_APPROPRIATE_VIDEO_CODEC_DOES_NOT_OPEN = -4;
	const int AGE_UI_VIDEO_APPROPRIATE_SOUND_CODEC_DOES_NOT_OPEN = -5;

}
