#include "../include/ageProgramClock.h"



double ProgramClock::all_program_time = 0.0f;
double ProgramClock::eleapsed_time_since_last_frame = 0.0f;
clock_t ProgramClock::point_time_val;


void ProgramClock::init()
{
	ProgramClock::point_time_val = clock();
}

void ProgramClock::update()
{
	clock_t finish_get_time_val = clock();
	ProgramClock::eleapsed_time_since_last_frame = (double)(finish_get_time_val - ProgramClock::point_time_val) / CLOCKS_PER_SEC;
	ProgramClock::all_program_time += ProgramClock::eleapsed_time_since_last_frame;
	ProgramClock::point_time_val = clock();
}
