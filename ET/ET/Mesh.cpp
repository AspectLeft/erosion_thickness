#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <iomanip>

ErosionThickness::Mesh::Mesh(std::string fileName)
{
	std::ifstream infile;
	infile.open(fileName);
	char head[4];
	int edgeNum; //useless
	infile >> head;
	infile >> vertexNum >> faceNum >> edgeNum;

	float x, y, z;
	for (int i = 0; i < vertexNum; ++i)
	{
		infile >> x >> y >> z;
		vertices.push_back(Point(x, y, z));
	}
	int facePoints;
	int pointIndex;
	for (int i = 0; i < faceNum; ++i)
	{
		infile >> facePoints;
		Face face;
		for (int j = 0; j < facePoints; ++j)
		{
			infile >> pointIndex; //starting from 0
			face.points.push_back(pointIndex);
		}
		faces.push_back(face);
	}

	infile.close();
}

void ErosionThickness::Mesh::output()
{
	std::ofstream outfile;
	outfile.open("mesh.off");


	outfile << "OFF" << std::endl;
	outfile << vertexNum << " " << faceNum << " " << 0 << std::endl;
	for (int i = 0; i < vertexNum; ++i)
	{
		Point &p = vertices[i];
		outfile << p.m_x << " " << p.m_y << " " << p.m_z << " " << std::endl;
	}
	for (int i = 0; i < faceNum; ++i)
	{
		Face &f = faces[i];
		outfile << f.points.size() << " ";
		for (int j = 0; j < f.points.size(); ++j)
		{
			outfile << f.points[j] << " ";
		}
		outfile << std::endl;
	}
	

	outfile.close();
}

ErosionThickness::MedialAxis::MedialAxis(std::string fileName, std::string rFile): Mesh(fileName)
{
	std::ifstream inFile;
	inFile.open(rFile);
	int vertexNum2;
	inFile >> vertexNum2;
	if (vertexNum2 != vertexNum)
	{
		std::cerr << "File not matching" << std::endl;
		goto quit;
	}
	float x, y, z, r;
	for (int i = 0; i < vertexNum; ++i)
	{
		inFile >> x >> y >> z >> r;
		radius.push_back(r);
	}

quit:
	inFile.close();
}

void ErosionThickness::MedialAxis::output()
{
	Mesh::output();
	std::ofstream outfile;
	outfile.open("radius.r");
	outfile << vertexNum << std::endl;
	for (int i = 0; i < vertexNum; ++i)
	{
		Point &p = vertices[i];
		outfile << p.m_x << " " << p.m_y << " " << p.m_z << " " << radius[i] << std::endl;
	}

	outfile.close();
}
