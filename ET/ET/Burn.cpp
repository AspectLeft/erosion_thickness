#include "Burn.h"
#include <iostream>
#include <fstream>
#include <iomanip>

ErosionThickness::Graph::Graph(const Mesh & mesh, float w1)
{
	w = w1;
	for (int i = 0; i < mesh.vertexNum; ++i)
	{
		nodes.push_back(Point(mesh.vertices[i])); //copy
	}
	std::vector<std::vector<std::vector<int>>> samplesPerFace;

	//Add nodes
	for (int i = 0; i < mesh.faceNum; ++i)
	{
		const Face &face = mesh.faces[i];
		int nodeNum = face.points.size();
		std::vector<std::vector<int>> samplesPerEdge;
		for (int j = 0; j < nodeNum; ++j)
		{
			std::vector<int> sampleIndex;
			const Point &startNode = mesh.vertices[face.points[j]];
			const Point &endNode = mesh.vertices[face.points[(j + 1) % nodeNum]];
			Vector edge = endNode - startNode;
			float length = edge.length();
			int sampleNum = length / w;
			if (sampleNum == 0)
				sampleNum = 1;
			Vector stride = edge * (1.0f / (float)(sampleNum + 1));
			for (int k = 0; k < sampleNum; ++k)
			{
				Point sample = startNode + (float)(k + 1) * stride;
				sampleIndex.push_back(nodes.size());
				nodes.push_back(sample);
			}
			samplesPerEdge.push_back(sampleIndex);
		}
		samplesPerFace.push_back(samplesPerEdge);

		

	}

	arcs = std::vector<std::vector<int>>(nodes.size());

	//Add arcs
	for (int i = 0; i < mesh.faceNum; ++i)
	{
		const Face &face = mesh.faces[i];
		int nodeNum = face.points.size();
		std::vector<std::vector<int>> samplesPerEdge = samplesPerFace[i];
		
		for (int j = 0; j < nodeNum; ++j) //vertices
		{
			int next = samplesPerEdge[j][0];
			int prev = samplesPerEdge[(j - 1 + nodeNum) % nodeNum].back();

			arcs[face.points[j]].push_back(next);
			arcs[face.points[j]].push_back(prev);
			for (int k = 0; k < nodeNum; ++k) //opposite edges
			{
				if (k == j || k == (j - 1 + nodeNum) % nodeNum)
					continue;
				const std::vector<int> &samples = samplesPerEdge[k];
				for (int m = 0; m < samples.size(); ++m)
				{
					arcs[face.points[j]].push_back(samples[m]);
				}
				if (k == (j + 1) % nodeNum)
					continue;
				arcs[face.points[j]].push_back(face.points[k]);
			}

			std::vector<int> sampleIndex = samplesPerEdge[j];
			int sampleNum = sampleIndex.size();
			for (int k = 0; k < sampleNum; ++k)
			{
				int prev;
				if (k == 0)
				{
					prev = face.points[j];
				}
				else
				{
					prev = sampleIndex[k - 1];
				}
				arcs[sampleIndex[k]].push_back(prev);

				int next;
				if (k == sampleNum - 1)
				{
					next = face.points[(j + 1) % nodeNum];
				}
				else
				{
					next = sampleIndex[k + 1];
				}
				arcs[sampleIndex[k]].push_back(next);

				for (int m = 0; m < nodeNum; ++m)
				{
					if (m == j)
						continue;
					for (int sample : samplesPerEdge[m])
					{
						arcs[sampleIndex[k]].push_back(sample);
					}
					if (m == (j + 1) % nodeNum)
						continue;
					arcs[sampleIndex[k]].push_back(face.points[m]);
				}
			}
		}
	}
}

void ErosionThickness::Graph::output()
{
	std::ofstream outfile;
	outfile.open("graph.txt");
	for (Point p : nodes)
	{
		outfile << p.m_x << " " << p.m_y << " " << p.m_z << std::endl;
	}
	for (std::vector<int> neighbors : arcs)
	{
		for (int neighbor : neighbors)
		{
			outfile << neighbor << " ";
		}
		outfile << std::endl;
	}

	outfile.close();

}
