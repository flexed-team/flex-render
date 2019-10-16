#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), vtexes_(), vfaces_(), tfaces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) 
	{
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            verts_.push_back(v);
        } 
	else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt.raw[i];
            vtexes_.push_back(vt);
        } 
	else if (!line.compare(0, 2, "f "))
	{
            std::vector<int> vf;
            std::vector<int> tf;
            int itrash, vidx, tidx;
            iss >> trash;
            while (iss >> vidx  >> trash >> tidx >> trash >> itrash) 
	    {
                tidx--; vidx--; // in wavefront obj all indices start at 1, not zero
                vf.push_back(vidx);
		tf.push_back(tidx);
            }
            vfaces_.push_back(vf);
	    tfaces_.push_back(tf);
        }
    }
    std::cerr << "# v# " << verts_.size() << " vt# " << vtexes_.size() << " f# "  << vfaces_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nvtexes() {
    return (int)vtexes_.size();
}

int Model::nvfaces() {
    return (int)vfaces_.size();
}

int Model::ntfaces() {
    return (int)tfaces_.size();
}

std::vector<int> Model::vface(int idx) {
    return vfaces_[idx];
}

std::vector<int> Model::tface(int idx) {
    return tfaces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2i Model::vtex(int i) {
    return Vec2i(vtexes_[i].x*diffusemap_.get_width(), vtexes_[i].y*diffusemap_.get_height());
}

void Model::load_texture(std::string filename, const char* suf, TGAImage &image){
    std::string texf(filename);
    size_t dot = texf.find_last_of(".");
    if(dot != std::string::npos){
    	texf = texf.substr(0, dot) + std::string(suf);
 	std::cerr<<"texture file"<<texf<<" lodaing "<<(image.read_tga_file(texf.c_str()) ? "ok" : "failed")<<std::endl;
	image.flip_vertically();
   }
}

TGAColor Model::getTex(Vec2i uv){
    return diffusemap_.get(uv.x, uv.y);	
    //return Vec2f(tfaces[i].x, tfaces[i].y);
}	
/*
Vec2f Model::getTexCoord(int i){
    //return diffusemap_.get(uv.x, uv.y);	
    //return Vec2f(tfaces[i].x, tfaces[i].y);
}*/
