#include "../include/ageWindow.h"



using namespace age;



Window* Window::active_window;



// Create a window with a specified width, height and title.
Window::Window(short window_width, short window_height, const char window_title[])
	: width(window_width), height(window_height)
{
	glfwInit();
	window = glfwCreateWindow(width, height, window_title, 0, 0);
	active_window = this;
	glfwMakeContextCurrent(window);
	glewInit();
}


// Closes window.
void Window::close()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

// Update the application window. Changes buffers and gets external actions.
void Window::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

// Set window to fullscreen mode.
void Window::set_fullscreen()
{
	const GLFWvidmode* glfw_video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = glfw_video_mode->width;
	height = glfw_video_mode->height;
	glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, glfw_video_mode->width, glfw_video_mode->height, 0);
	glViewport(0, 0, width, height);
}

// Set camera aspects ratio based on window dimensions.
void Window::add_camera(Camera *camera)
{
	camera->set_aspects_ratio(width, height);
}
