#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

// TODO: remove obsolete properties and methods
// TODO: rename properties and methods
class Model {
private:
	// Values
	/// Model verticies array
	std::vector<Vec3f> verts_;
	/// Texture verticies array
	std::vector<Vec2f> vtexes_;
	/// Model normal vectors array 
	std::vector<Vec3f> vnormals_;
	
	// String indexes which refer to values
	/// Array of all faces string indexes, which refer to model verticies
	///
	/// Taken from `f` string
	std::vector<std::vector<int> > vfaces_;
	
	/// Array of all faces string indexes, which refer to textrure verticies
	///
	/// Taken from `f` string
	std::vector<std::vector<int> > tfaces_;
	
	/// Array of all faces string indexes, which refer to model normal vectors
	///
	/// Taken from `f` string
	std::vector<std::vector<int> > nfaces_;

	TGAImage diffusemap_;
	void load_texture(std::string filename, const char* suf, TGAImage &image);
public:
	Model(const char *filename);
	~Model();

	/// Returns model vertex array size
	int nverts();		
	/// Returns textures vertexes array size
	int nvtexes();
	/// Returns model normal vectors array size
	int nvnormals();
	
	/// Returns model verticies array size
	int nvfaces();
	/// @deprecated obsolete
	int ntfaces();
	/// @deprecated obsolete
	int nnfaces();

	/// Gets model vertex
	/// @param i — string index
	Vec3f vert(int i); 
	/// Gets texture vertex
	/// @param i — string index
	Vec2i vtex(int i);
	/// Gets model normal vector
	/// @param i — string index
	Vec3f norm(int i);
	
	/// Gets color of pixel by its coords
	TGAColor getTex(Vec2i uv);
	//Vec2f getTexCoord(int i);

	/// Gets model vertex from face
	/// @param idx — string index of face
	std::vector<int> vface(int idx);
	/// Gets texture vertex from face
	/// @param idx — string index of face
	std::vector<int> tface(int idx);
	/// Gets normal vector from face
	/// @param idx — string index of face
	std::vector<int> nface(int idx);
};

#endif //__MODEL_H__