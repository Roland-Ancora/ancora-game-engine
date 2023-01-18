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
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "ageCamera.h"



namespace age {

	class Window {
		GLFWwindow* window;
		short width, height;
	public:
		Window(short window_width, short window_height, const char window_title[]);

		int is_close() { return glfwWindowShouldClose(window); }
		short get_width() { return width; }
		short get_height() { return height; }

		void close();
		void update();
		void set_fullscreen();
		void add_camera(Camera* camera);

		operator GLFWwindow* () const { return window; }
	};

}
