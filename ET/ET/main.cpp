#include <iostream>
#include <fstream>
#include <iomanip>
#include "Mesh.h"
#include "Burn.h"

using namespace ErosionThickness;

int main()
{
	std::cout << "hello" << std::endl;

	//Mesh mesh("G:/kc4i/Erosion Thickness/et/dataset/bird.off");
	//MedialAxis m("G:/kc4i/Erosion Thickness/et/dataset/medial-bird-0.008.off", "G:/kc4i/Erosion Thickness/et/dataset/medial-bird-0.008.r");
	//m.output();
	Mesh cube("G:/kc4i/Erosion Thickness/et/dataset/square.off");
	Graph g(cube, 0.5f);
	g.output();
}