#include "../include/ageInputEventsControler.h"



using namespace age;

bool InputEventsControler::is_any_mouse_key_pressed = false;
std::vector<int> InputEventsControler::keys_now_pressed;
int InputEventsControler::key_now_pressed = -1;
bool InputEventsControler::frame_to_work_keys = true;
bool InputEventsControler::is_scroll_callback_called = false;
float InputEventsControler::scroll_x = 0.0f;
float InputEventsControler::scroll_y = 0.0f;





void InputEventsControler::Init(Window* win)
{
	glfwSetScrollCallback(*win, scroll_callback);
}

void InputEventsControler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll_x = xoffset;
	scroll_y = yoffset;
	is_scroll_callback_called = true;
}

bool InputEventsControler::is_mouse_button_pressed_and_released(Window window, int key)
{
	if (glfwGetMouseButton(window, key) == GLFW_PRESS) {
		key_now_pressed = key;
	}
	else if (key_now_pressed == key) {
		return true;
	}
	return false;
}

glm::vec3 InputEventsControler::get_mouse_position_in_world_coords(Window& window, Camera& camera)
{
	int vievport[4];
	double mouse_pos_x, mouse_pos_y;
	GLfloat pixels;
	glGetIntegerv(GL_VIEWPORT, vievport);
	glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);
	glReadPixels(int(mouse_pos_x), window.get_height() - int(mouse_pos_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixels);
	glm::vec3 pr_vec3 = glm::unProject(glm::vec3(mouse_pos_x, window.get_height() - mouse_pos_y, pixels),
		*camera.get_view_matrix(),
		*camera.get_proj_matrix(),
		glm::vec4(vievport[0], vievport[1], vievport[2], vievport[3]));
	return glm::vec3(pr_vec3.x, pr_vec3.y, pr_vec3.z);
}

void InputEventsControler::clear_frame(Window window)
{ 
	if (is_scroll_callback_called) {
		is_scroll_callback_called = false;
	}
	else {
		scroll_y = 0.0f;
		scroll_x = 0.0f;
	}
	if (key_now_pressed != -1 && glfwGetMouseButton(window, key_now_pressed) == GLFW_RELEASE && frame_to_work_keys) {
		frame_to_work_keys = false;
	}
	else {
		key_now_pressed = -1;
		frame_to_work_keys = true;
	}
}

