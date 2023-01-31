//#####################################################################//
//
//							PorgramClock
//	
//		The PorgramClock class is responsible for calculation of
//	program running time and time elapsed per frame.
// 
//		The PorgramClock class has external dependencies: STD.
//
//
//#####################################################################//



#pragma once
#include <ctime>



class ProgramClock {
	ProgramClock();
	static double all_program_time;
	static double eleapsed_time_since_last_frame;
	static clock_t point_time_val;
public:
	static void init();
	static void update();
	static double get_all_program_time() { return ProgramClock::all_program_time; }
	static double get_eleapsed_frame_time() { return ProgramClock::eleapsed_time_since_last_frame; }
};
