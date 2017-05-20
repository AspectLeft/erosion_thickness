#ifndef __BURN_H__
#define __BURN_H__

#include <vector>
#include "Util.h"
#include "Mesh.h"

namespace ErosionThickness
{
	class Graph
	{
	public:
		float w; //max distance between 2 nodes
		std::vector<Point> nodes;
		std::vector<std::vector<int>> arcs;

		Graph(const Mesh& mesh, float w1);

		void output();
	};
}

#endif