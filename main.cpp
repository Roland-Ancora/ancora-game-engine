#include "include/ageWindow.h"



using namespace age;

int main()
{
	Window main_window(1366, 768, "Ancora Game Engine");

	while (!main_window.is_close()) {
		main_window.update();
	}
	
	main_window.close();

	return 0;
}
