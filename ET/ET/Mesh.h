#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>
#include "Util.h"

namespace ErosionThickness
{
	struct Mesh
	{
		int vertexNum, faceNum;
		std::vector<Point> vertices;
		std::vector<Face> faces;

		Mesh(std::string fileName);

		void output();
	};

	struct MedialAxis : public Mesh
	{
		std::vector<float> radius;
		MedialAxis(std::string fileName, std::string rFile);

		void output();
	};
}


#endif
