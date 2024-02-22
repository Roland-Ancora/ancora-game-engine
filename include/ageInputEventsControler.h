//#####################################################################//
//
//							InputEventsControler
//	
//		The InputEventsControler class is responsible for the handing 
//	input events, like mouse, keyboard and other. 
// 
//		This module has external dependencies: GLFW, GLM, STD.
//		This module has dependencies: ageWindow.
//
//
//#####################################################################//



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../depends/glm/glm.hpp>
#include <vector>

#include "ageWindow.h"



namespace age {

	class InputEventsControler {
		InputEventsControler();
		static bool is_any_mouse_key_pressed;
		static std::vector<int> keys_now_pressed;
		static int key_now_pressed;
		static bool frame_to_work_keys;
		static bool is_scroll_callback_called;
		static float scroll_x, scroll_y;

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	public:
		static float get_scroll_x() { return scroll_x; }
		static float get_scroll_y() { return scroll_y; }

		static void Init();
		static bool is_mouse_button_pressed_and_released(int key);
		static void clear_frame();
	};

}
