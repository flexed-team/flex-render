#pragma once

#include <flex-math.h>

#include "core.h"

#include "model.h"
#include "tgaimage.h"

namespace FR
{	
	namespace colors
	{
		const TGAColor white = TGAColor(255, 255, 255, 255);
		const TGAColor red = TGAColor(255, 0, 0, 255);
		const TGAColor green = TGAColor(0, 255, 0, 255);
	}

	struct FR_API FaceData
	{
		std::vector<int> vface;
		std::vector<int> tface;
		std::vector<int> nface;

		FE::Vec3i screen_coords[3];
		FE::Vec3f world_coords[3];

		FE::Vec2i tex_coords[3];

		FE::Vec3f normal_coords[3];
	};

	// Screen params--------
	const int width = 800;
	const int height = 800;
	const int depth = 255;
	// ---------------------

	FE::Vec3f light_dir(0, 0, 1);
	FE::Vec3f camera(0, 0, 3);
	int* zbuffer = NULL;
	bool phong = true;


	extern "C" FR_API FE::Matrix<float> viewport(int, int, int, int, const int);
	
	extern "C" FR_API void FR_init();

	extern "C" FR_API TGAImage generate_image();

	extern "C" FR_API void line(FE::Vec3i, FE::Vec3i, std::vector<FE::Vec3i>&);
	extern "C" FR_API void triangle(FaceData, TGAImage&, Model*);

	extern "C" FR_API void draw_image(Model*, const char*);
}