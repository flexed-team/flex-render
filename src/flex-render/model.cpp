#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "model.h"

namespace FR {
    Model::Model(const char* filename) : verts_(), vtexes_(), vfaces_(), tfaces_() {
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        if (in.fail()) return;
        std::string line;
        while (!in.eof()) {
            std::getline(in, line);
            std::istringstream iss(line.c_str());
            char trash;
            if (!line.compare(0, 2, "v "))
            {
                iss >> trash;
                FE::Vec3f v;
                for (int i = 0; i < 3; i++) iss >> v.raw[i];
                verts_.push_back(v);
            }
            else if (!line.compare(0, 3, "vt ")) {
                iss >> trash >> trash;
                FE::Vec2f vt;
                for (int i = 0; i < 2; i++) iss >> vt.raw[i];
                vtexes_.push_back(vt);
            }
            else if (!line.compare(0, 3, "vn ")) {
                iss >> trash >> trash;
                FE::Vec3f vn;
                for (int i = 0; i < 3; i++) iss >> vn.raw[i];
                vnormals_.push_back(vn);
            }
            else if (!line.compare(0, 2, "f "))
            {
                std::vector<int> vf;
                std::vector<int> tf;
                std::vector<int> nf;
                int vidx, tidx, nidx;
                iss >> trash;
                while (iss >> vidx >> trash >> tidx >> trash >> nidx)
                {
                    tidx--; vidx--; nidx--; // in wavefront obj all indices start at 1, not zero
                    vf.push_back(vidx);
                    tf.push_back(tidx);
                    nf.push_back(nidx);
                }
                vfaces_.push_back(vf);
                tfaces_.push_back(tf);
                nfaces_.push_back(nf);
            }
        }
        std::cerr << "# v# " << verts_.size() << " vt# " << vtexes_.size() << " vn# " << vnormals_.size() << " f# " << vfaces_.size() << std::endl;
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

    int Model::nvnormals() {
        return (int)vnormals_.size();
    }

    int Model::nvfaces() {
        return (int)vfaces_.size();
    }

    int Model::ntfaces() {
        return (int)tfaces_.size();
    }

    int Model::nnfaces() {
        return (int)nfaces_.size();
    }

    std::vector<int> Model::vface(int idx) {
        return vfaces_[idx];
    }

    std::vector<int> Model::tface(int idx) {
        return tfaces_[idx];
    }

    std::vector<int> Model::nface(int idx) {
        return nfaces_[idx];
    }

    FE::Vec3f Model::vert(int i) {
        return verts_[i];
    }

    FE::Vec2i Model::vtex(int i) {
        return FE::Vec2i(vtexes_[i].x * diffusemap_.get_width(), vtexes_[i].y * diffusemap_.get_height());
    }

    FE::Vec3f Model::norm(int i) {
        return vnormals_[i];
    }

    void Model::load_texture(std::string filename, const char* suf, TGAImage& image) {
        std::string texf(filename);
        size_t dot = texf.find_last_of(".");
        if (dot != std::string::npos) {
            texf = texf.substr(0, dot) + std::string(suf);
            std::cerr << "texture file " << texf << " lodaing " << (image.read_tga_file(texf.c_str()) ? "ok" : "failed") << std::endl;
            image.flip_vertically();
        }
    }

    TGAColor Model::getTex(FE::Vec2i uv) {
        return diffusemap_.get(uv.x, uv.y);
        //return Vec2f(tfaces[i].x, tfaces[i].y);
    }
    /*
    Vec2f Model::getTexCoord(int i){
        //return diffusemap_.get(uv.x, uv.y);
        //return Vec2f(tfaces[i].x, tfaces[i].y);
    }*/

}
