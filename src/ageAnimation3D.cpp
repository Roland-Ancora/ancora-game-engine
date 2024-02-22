#include "../include/ageAnimation3D.h"



using namespace age;



Animation3D::~Animation3D()
{
	delete[] nodes;
}
