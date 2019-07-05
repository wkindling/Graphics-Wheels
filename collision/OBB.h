#ifndef OBB_H
#define OBB_H

#include <Eigen/Dense>
#include <Eigen/Sparse>


class OBB
{
public:
	OBB();
	OBB(Eigen::Vector3d _center, double lx, double ly, double lz);
	virtual ~OBB();
	
	bool collision(OBB* target);
	void rotate(Eigen::Vector3d euler);
	void draw();
	void updateVertex();
	
	Eigen::Vector2d project(Eigen::Vector3d axis);


public:
	Eigen::Vector3d center;
	Eigen::Vector3d director[3];
	double length[3];
	Eigen::Vector3d vertices[8];

	Eigen::Vector3d color;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif