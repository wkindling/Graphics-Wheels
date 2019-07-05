#include "OBB.h"
#include <freeglut.h>
#include <iostream>
#include <vector>

using namespace Eigen;
using namespace std;

OBB::OBB() {}

OBB::OBB(Vector3d _center, double lx, double ly, double lz)
{
	center = _center;
	
	length[0] = lx;
	length[1] = ly;
	length[2] = lz;

	director[0] = Vector3d(1, 0, 0);
	director[1] = Vector3d(0, 1, 0);
	director[2] = Vector3d(0, 0, 1);

	color = Vector3d(1, 1, 1);

	updateVertex();
}

OBB::~OBB() {}

bool OBB::collision(OBB* target)
{
	vector<Vector3d> axises;
	for (int i = 0; i < 3; i++)
	{
		axises.push_back(this->director[i]);
		axises.push_back(target->director[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3d axis = this->director[i].cross(target->director[j]);
			if (axis.norm()!=0)
			{
				axis.normalize();
				axises.push_back(axis);
			}
		}
	}
	
	for (int i = 0; i < axises.size(); i++)
	{
		Vector2d proj1 = this->project(axises[i]);
		Vector2d proj2 = target->project(axises[i]);
		
		if (max(proj1(0), proj2(0)) > min(proj1(1), proj2(1)))
		{
			color = Vector3d(1, 1, 1);
			return false;
		}
	}

	color = Vector3d(1, 0, 0);
	return true;
}

void OBB::rotate(Vector3d euler)
{
	Matrix3d RotX, RotY, RotZ;
	double x = euler.x();
	double y = euler.y();
	double z = euler.z();

	RotX << 1, 0, 0,
		0, cos(x), -sin(x),
		0, sin(x), cos(x);

	RotY << cos(y), 0, sin(y),
		0, 1, 0,
		-sin(y), 0, cos(y);

	RotZ << cos(z), -sin(z), 0,
		sin(z), cos(z), 0,
		0, 0, 1;

	Matrix3d local_rotation = RotZ * RotX*RotY;

	for (int i=0;i<3;i++)
	{
		director[i] = local_rotation * director[i];
	}

	updateVertex();
}

void OBB::updateVertex()
{
	vertices[0] = center - director[0] * length[0] - director[1] * length[1] - director[2] * length[2];
	vertices[1] = center - director[0] * length[0] + director[1] * length[1] - director[2] * length[2];
	vertices[2] = center - director[0] * length[0] + director[1] * length[1] + director[2] * length[2];
	vertices[3] = center - director[0] * length[0] - director[1] * length[1] + director[2] * length[2];
	vertices[4] = center + director[0] * length[0] - director[1] * length[1] + director[2] * length[2];
	vertices[5] = center + director[0] * length[0] + director[1] * length[1] + director[2] * length[2];
	vertices[6] = center + director[0] * length[0] + director[1] * length[1] - director[2] * length[2];
	vertices[7] = center + director[0] * length[0] - director[1] * length[1] - director[2] * length[2];
}

Vector2d OBB::project(Vector3d axis)
{
	double min = 10000000;
	double max = -10000000;
	axis.normalize();

	for (int i = 0; i < 8; i++)
	{
		double proj = vertices[i].dot(axis);
		if (proj > max) max = proj;
		if (proj < min) min = proj;
	}
	return Vector2d(min, max);
}


void OBB::draw()
{
	glColor3d(color.x(), color.y(), color.z());
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3d(vertices[0].x(), vertices[0].y(), vertices[0].z());
	glVertex3d(vertices[1].x(), vertices[1].y(), vertices[1].z());

	glVertex3d(vertices[0].x(), vertices[0].y(), vertices[0].z());
	glVertex3d(vertices[3].x(), vertices[3].y(), vertices[3].z());

	glVertex3d(vertices[0].x(), vertices[0].y(), vertices[0].z());
	glVertex3d(vertices[7].x(), vertices[7].y(), vertices[7].z());

	glVertex3d(vertices[5].x(), vertices[5].y(), vertices[5].z());
	glVertex3d(vertices[4].x(), vertices[4].y(), vertices[4].z());

	glVertex3d(vertices[5].x(), vertices[5].y(), vertices[5].z());
	glVertex3d(vertices[2].x(), vertices[2].y(), vertices[2].z());

	glVertex3d(vertices[5].x(), vertices[5].y(), vertices[5].z());
	glVertex3d(vertices[6].x(), vertices[6].y(), vertices[6].z());

	glVertex3d(vertices[3].x(), vertices[3].y(), vertices[3].z());
	glVertex3d(vertices[4].x(), vertices[4].y(), vertices[4].z());

	glVertex3d(vertices[3].x(), vertices[3].y(), vertices[3].z());
	glVertex3d(vertices[2].x(), vertices[2].y(), vertices[2].z());

	glVertex3d(vertices[6].x(), vertices[6].y(), vertices[6].z());
	glVertex3d(vertices[7].x(), vertices[7].y(), vertices[7].z());

	glVertex3d(vertices[6].x(), vertices[6].y(), vertices[6].z());
	glVertex3d(vertices[1].x(), vertices[1].y(), vertices[1].z());

	glVertex3d(vertices[4].x(), vertices[4].y(), vertices[4].z());
	glVertex3d(vertices[7].x(), vertices[7].y(), vertices[7].z());

	glVertex3d(vertices[2].x(), vertices[2].y(), vertices[2].z());
	glVertex3d(vertices[1].x(), vertices[1].y(), vertices[1].z());
	glEnd();
	
	/*
	glLineWidth(2.0);

	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(center.x(), center.y(), center.z());
	glVertex3d((center + director[0]).x(), (center + director[0]).y(), (center + director[0]).z());
	glEnd();

	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(center.x(), center.y(), center.z());
	glVertex3d((center + director[1]).x(), (center + director[1]).y(), (center + director[1]).z());	glEnd();
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(center.x(), center.y(), center.z());
	glVertex3d((center + director[2]).x(), (center + director[2]).y(), (center + director[2]).z());
	glEnd();
	*/
}
