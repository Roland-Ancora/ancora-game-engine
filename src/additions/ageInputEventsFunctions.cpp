#include "../../include/additions/ageInputEventsFunctions.h"



double* age::get_click_position_in_world_coords(Window& window, Camera3D& camera)
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
	return new double[3] { pr_vec3.x, pr_vec3.y, pr_vec3.z };
}
