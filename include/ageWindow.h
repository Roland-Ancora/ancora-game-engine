//##########################################################################//
//
//								Window
//	
//		The Window class is responsible for the creating and destroy-
//	ing the window, his dimensions, double buffering and poll events.
// 
//		The Window class has external dependencies: GLFW, OpenGL.
//		The Window class has dependencies: ageCamera.
// 
//		The Window class has Camera depend because Camera requires
//	aspects ratio of program window. After Camera creating, need to
//	send her in add_camera function, where Window calls set_sides_aspect 
//	function from Camera class with himself dimensions.
// 
//		The Window class has GLFWwindow* operator. That's need for backward
//	compatibillity with GLFW functions.
// 
//
//##########################################################################//



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ageCamera.h"
#include "ageCONSTANTS.h"



namespace age {

	class Window {
		GLFWwindow* window;
		int width, height;
		static Window* active_window;
	public:
		Window(int window_width, int window_height, const char window_title[]);
		static Window* get_active_window() { return active_window; }
	public:
		int is_close() { return glfwWindowShouldClose(window); }
		int get_width() { return width; }
		int get_height() { return height; }
	public:
		void close();
		void update();
		void set_fullscreen();
		void add_camera(Camera* camera);
		glm::vec2 get_cursor_position();
		void set_cursor_position(float x, float y);
		void cursor_hide();
		void cursor_show();
	public:
		operator GLFWwindow* () const { return window; }
	};

}
