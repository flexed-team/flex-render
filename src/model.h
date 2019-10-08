#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> vtexes_;

	std::vector<std::vector<int> > vfaces_;
	std::vector<std::vector<int> > tfaces_;
public:
	Model(const char *filename);
	~Model();

	int nverts();		//returns vertex array size
	int nvtexes();		//returns textures vertexes array size

	int nvfaces();		//returns textures faces array size
	int ntfaces();		// -//-

	Vec3f vert(int i);      //returns vertex
	Vec3f vtex(int i);	//return texture vertex

	std::vector<int> vface(int idx);
	std::vector<int> tface(int idx);
};

#endif //__MODEL_H__
