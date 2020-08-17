#pragma once

#include <vector>
#include <flex-math.h>

#include "tgaimage.h"
#include "core.h"

namespace FR {
	class FR_API Model {
	private:
		std::vector<FE::Vec3f> verts_;
		std::vector<FE::Vec2f> vtexes_;
		std::vector<FE::Vec3f> vnormals_;

		std::vector<std::vector<int> > vfaces_;
		std::vector<std::vector<int> > tfaces_;
		std::vector<std::vector<int> > nfaces_;

		TGAImage diffusemap_;
		void load_texture(std::string filename, const char* suf, TGAImage& image);
	public:
		Model(const char* filename);
		~Model();

		int nverts();		//returns vertex array size
		int nvtexes();		//returns textures vertexes array size
		int nvnormals();	//returns normal vectors array size

		int nvfaces();		//returns textures faces array size
		int ntfaces();		// -//-
		int nnfaces();		// -//-

		FE::Vec3f vert(int i);      //returns vertex
		FE::Vec2i vtex(int i);	//returns texture vertex
		FE::Vec3f norm(int i);	//returns normal vector

		TGAColor getTex(FE::Vec2i uv);	//returns texture Vec2i
		//Vec2f getTexCoord(int i);

		std::vector<int> vface(int idx);
		std::vector<int> tface(int idx);
		std::vector<int> nface(int idx);
	};
}
