#include "ageWindow.h"



using namespace age;



Window* Window::active_window = 0;



// Create a window with a specified width, height and title.
Window::Window(int window_width, int window_height, const char window_title[])
	: width(window_width), height(window_height)
{
	if (active_window != 0)
		exit(AGE_WINDOW_OBJECT_OVERLAPPING_ERROR);
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
	camera->set_aspects_ratio(static_cast<float>(width), static_cast<float>(height));
}

glm::vec2 Window::get_cursor_position()
{
	double pos_x, pos_y;
	glfwGetCursorPos(window, &pos_x, &pos_y);
	return glm::vec2(pos_x, pos_y);
}
