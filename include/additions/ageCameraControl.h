#include "ageCamera.h"
#include "ageInputEventsControler.h"
#include "ageWindow.h"



namespace age {

	struct camera_control_3d_frame_data {
		float pos_x_last = 900.0f, pos_y_last = 500.0f;
	};

	camera_control_3d_frame_data _CAMERA_CONTROL_3D_BASIC_DATA;

	void camera_control_3d_basic(Camera3D& camera)
	{
		Window* win = Window::get_active_window();

		glm::vec2 cursor_pos = win->get_cursor_position();
		float x = _CAMERA_CONTROL_3D_BASIC_DATA.pos_x_last - cursor_pos.x;
		float y = _CAMERA_CONTROL_3D_BASIC_DATA.pos_y_last - cursor_pos.y;
		float sensetive = 200.0f;
		if (camera.get_rotation().y + glm::radians(y / sensetive) >= 1.407 && glm::radians(y / sensetive) > 0.0f)
			camera.rotate(x / sensetive, 0.0f);
		else if (camera.get_rotation().y + glm::radians(y / sensetive) <= -1.407 && glm::radians(y / sensetive) < 0.0f)
			camera.rotate(x / sensetive, 0.0f);
		else
			camera.rotate(x / sensetive, y / sensetive);
		_CAMERA_CONTROL_3D_BASIC_DATA.pos_x_last = cursor_pos.x;
		_CAMERA_CONTROL_3D_BASIC_DATA.pos_y_last = cursor_pos.y;

		win->set_cursor_position(win->get_width() / 2.0f, win->get_height() / 2.0f);
		cursor_pos = win->get_cursor_position();
		_CAMERA_CONTROL_3D_BASIC_DATA.pos_x_last = cursor_pos.x;
		_CAMERA_CONTROL_3D_BASIC_DATA.pos_y_last = cursor_pos.y;
	}

}
