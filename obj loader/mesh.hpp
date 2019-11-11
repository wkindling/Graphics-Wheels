// Simple Mesh
//
// Created by Jiahao Wen on 2019/11/11
//
#ifndef MESH_H
#define MESH_H

// Eigen Include
#include <Eigen/Dense>
#include <Eigen/Sparse>
// Std Include
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

struct Vertex;
struct Edge;
struct Face;

struct Vertex
{
	Eigen::Vector3d Position;
	std::vector<Face*> adjf;
	std::vector<Edge*> adje;

	int index;
};

struct Edge
{
	Vertex* v[2];
	Face* adjf[2];

	int index;
};

struct  Face
{
	Vertex* v[3];
	Eigen::Vector3d Normal;

	int index;
};

class Mesh
{
public:
	Mesh() {}
	virtual ~Mesh() {}

	bool load(std::string Path)
	{
		if (Path.substr(Path.size() - 4, 4) != ".obj")
		{
			std::cout << "Only support {.obj} file format." << std::endl;
			return false;
		}

		std::ifstream file(Path);
		if (!file.is_open())
		{
			std::cout << "Cannot open target file." << std::endl;
			return false;
		}

		vertices.clear();
		edges.clear();
		faces.clear();

		std::string line;
		while (std::getline(file, line))
		{
			std::vector<std::string> stream;
			std::string end_mark = " ";
			std::string value = "";

			line = line.append(end_mark);
			int flag = 0;

			for (unsigned int i = 0; i < line.length(); i++)
			{
				char ch = line[i];
				if (ch != ' ')
				{
					value += ch;
					flag = 1;
				}
				else
				{
					if (flag) stream.push_back(value);
					flag = 0;
					value = "";
				}
			}

			if (stream.size() == 0) continue;
			else if (stream[0] == "v")
			{
				Vertex* vertex = new Vertex();
				vertex->Position = Eigen::Vector3d(atof(stream[1].c_str()), atof(stream[2].c_str()), atof(stream[3].c_str()));
				vertex->index = vertices.size();
				vertices.push_back(vertex);
			}
			else if (stream[0] == "vn")
			{
				Eigen::Vector3d nor(atof(stream[1].c_str()), atof(stream[2].c_str()), atof(stream[3].c_str()));
				vnor.push_back(nor);
			}
			else if (stream[0] == "f")
			{
				int vertex_index[3];
				int vnor_index[3];

				for (int i = 1; i <= 3; i++)
				{
					std::string v_index = "";
					std::string vn_index = "";
					int count = 0;
					for (unsigned int j = 0; j < stream[i].length(); j++)
					{
						char ch = stream[i][j];
						if (ch != '/' && count == 0) v_index += ch;
						else if (ch != '/' && count == 2) vn_index += ch;
						else if (ch == '/') count++;
					}
					vertex_index[i - 1] = atof(v_index.c_str()) - 1;
					vnor_index[i - 1] = atof(vn_index.c_str()) - 1;
				}

				// Face
				Face* face = new Face();
				face->v[0] = vertices[vertex_index[0]];
				face->v[1] = vertices[vertex_index[1]];
				face->v[2] = vertices[vertex_index[2]];

				Eigen::Vector3d f_nor = (face->v[1]->Position - face->v[0]->Position).cross(face->v[2]->Position - face->v[1]->Position); f_nor.normalize();
				face->Normal = f_nor.dot(vnor[vnor_index[0]]) > 0 ? f_nor : -f_nor;

				face->index = faces.size();
				faces.push_back(face);

				// Vertex
				face->v[0]->adjf.push_back(face);
				face->v[1]->adjf.push_back(face);
				face->v[2]->adjf.push_back(face);

				// Edge
				Edge* edge0 = new Edge();
				edge0->v[0] = face->v[0]; edge0->v[1] = face->v[1];
				if (find_in(face->v[0], edge0) == nullptr && find_in(face->v[1], edge0) == nullptr)
				{
					face->v[0]->adje.push_back(edge0); 
					face->v[1]->adje.push_back(edge0);
					edge0->adjf[0] = face;
					edge0->index = edges.size();
					edges.push_back(edge0);
				}
				else
				{
					Edge* old_edge = find_in(face->v[0], edge0);
					old_edge->adjf[1] = face;
				}

				Edge* edge1 = new Edge();
				edge1->v[0] = face->v[1]; edge1->v[1] = face->v[2];
				if (find_in(face->v[1], edge1) == nullptr && find_in(face->v[2], edge1) == nullptr)
				{
					face->v[1]->adje.push_back(edge1);
					face->v[2]->adje.push_back(edge1);
					edge1->adjf[0] = face;
					edge1->index = edges.size();
					edges.push_back(edge1);
				}
				else
				{
					Edge* old_edge = find_in(face->v[1], edge1);
					old_edge->adjf[1] = face;
				}

				Edge* edge2 = new Edge();
				edge2->v[0] = face->v[2]; edge2->v[1] = face->v[0]; edge2->index = edges.size();
				if (find_in(face->v[2], edge2) == nullptr && find_in(face->v[0], edge2) == nullptr)
				{
					face->v[2]->adje.push_back(edge2);
					face->v[0]->adje.push_back(edge2);
					edge2->adjf[0] = face;
					edges.push_back(edge2);
				}
				else
				{
					Edge* old_edge = find_in(face->v[2], edge2);
					old_edge->adjf[1] = face;
				}
			}
		}
		return true;
	}

public:
	std::vector<Vertex*> vertices;
	std::vector<Edge*> edges;
	std::vector<Face*> faces;

private:
	std::vector<Eigen::Vector3d> vnor;

	Edge* find_in(Vertex* vert, Edge* edge)
	{
		for (int i = 0; i < vert->adje.size(); i++)
		{
			if ((vert->adje[i]->v[0] == edge->v[0] && vert->adje[i]->v[1] == edge->v[1]) || (vert->adje[i]->v[0] == edge->v[1] && vert->adje[i]->v[1] == edge->v[0]))
			{
				return vert->adje[i];
			}
		}
		return nullptr;
	}
};

#endif