#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec2f> vtexes_;
	std::vector<Vec3f> vnormals_;

	std::vector<std::vector<int> > vfaces_;
	std::vector<std::vector<int> > tfaces_;
	std::vector<std::vector<int> > nfaces_;

	TGAImage diffusemap_;
	void load_texture(std::string filename, const char* suf, TGAImage &image);
public:
	Model(const char *filename);
	~Model();

	int nverts();		//returns vertex array size
	int nvtexes();		//returns textures vertexes array size
	int nvnormals();	//returns normal vectors array size
	
	int nvfaces();		//returns textures faces array size
	int ntfaces();		// -//-
	int nnfaces();		// -//-

	Vec3f vert(int i);      //returns vertex
	Vec2i vtex(int i);	//returns texture vertex
	Vec3f norm(int i);	//returns normal vector
	
	TGAColor getTex(Vec2i uv);	//returns texture Vec2i
	//Vec2f getTexCoord(int i);

	std::vector<int> vface(int idx);
	std::vector<int> tface(int idx);
	std::vector<int> nface(int idx);
};

#endif //__MODEL_H__
