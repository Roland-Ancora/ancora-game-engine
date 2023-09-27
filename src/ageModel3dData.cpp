#include "../include/ageModel3dData.h"



using namespace age;



Model3dData::~Model3dData()
{
	delete[] vertices;
	delete[] texture_coords;
	delete[] normals;
	delete[] indices;
}


Model3dGroupDataNode::~Model3dGroupDataNode()
{
	delete[] childs;
}


Model3dGroupData::~Model3dGroupData()
{
	delete[] childs;
}
