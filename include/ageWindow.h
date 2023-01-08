//#####################################################################//
//
//								Window
//	
//		The Window class is responsible for the creating and destroy-
//	ing the window, his dimensions, double buffering and poll events.
// 
//		The Window class has external dependencies: GLFW, OpenGL.
// 
//
//#####################################################################//



#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"



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
};

}
