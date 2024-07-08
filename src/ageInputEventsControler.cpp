#include "../include/ageInputEventsControler.h"



using namespace age;



bool InputEventsControler::is_any_mouse_key_pressed = false;
std::vector<int> InputEventsControler::keys_now_pressed;
int InputEventsControler::mouse_key_now_pressed = -1;
int InputEventsControler::key_now_pressed = -1;
bool InputEventsControler::frame_to_work_mouse_keys = true;
bool InputEventsControler::frame_to_work_keys = true;
bool InputEventsControler::is_scroll_callback_called = false;
float InputEventsControler::scroll_x = 0.0f;
float InputEventsControler::scroll_y = 0.0f;



void InputEventsControler::Init()
{
	glfwSetScrollCallback(*Window::get_active_window(), scroll_callback);
}

void InputEventsControler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll_x = xoffset;
	scroll_y = yoffset;
	is_scroll_callback_called = true;
}

bool InputEventsControler::is_mouse_button_pressed_and_released(int key)
{
	if (glfwGetMouseButton(*Window::get_active_window(), key) == GLFW_PRESS)
		mouse_key_now_pressed = key;
	else if (mouse_key_now_pressed == key)
		return true;
	return false;
}

bool InputEventsControler::is_key_pressed_and_released(int key)
{
	if (glfwGetKey(*Window::get_active_window(), key) == GLFW_PRESS)
		key_now_pressed = key;
	else if (key_now_pressed == key)
		return true;
	return false;
}

void InputEventsControler::clear_frame()
{ 
	if (is_scroll_callback_called)
		is_scroll_callback_called = false;
	else {
		scroll_y = 0.0f;
		scroll_x = 0.0f;
	}
	if (mouse_key_now_pressed != -1 && glfwGetMouseButton(*Window::get_active_window(), mouse_key_now_pressed) == GLFW_RELEASE && frame_to_work_mouse_keys)
		frame_to_work_mouse_keys = false;
	else {
		mouse_key_now_pressed = -1;
		frame_to_work_mouse_keys = true;
	}
	if (key_now_pressed != -1 && glfwGetKey(*Window::get_active_window(), key_now_pressed) == GLFW_RELEASE && frame_to_work_keys)
		frame_to_work_keys = false;
	else {
		key_now_pressed = -1;
		frame_to_work_keys = true;
	}
}
