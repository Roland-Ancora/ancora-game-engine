#include "ageModel3dData.h"



using namespace age;



Model3dData::~Model3dData()
{
	delete[] vertices;
	delete[] texture_coords;
	delete[] normals;
	delete[] indices;
}

void Model3dData::clear()
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

void Model3dGroupDataNode::clear()
{
	delete[] childs;
}





Model3dGroupData::~Model3dGroupData()
{
	childs_count = 0;
	delete[] childs;
}

void Model3dGroupData::clear()
{
	childs_count = 0;
	delete[] childs;
}
