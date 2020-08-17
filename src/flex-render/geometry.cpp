#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <limits>

#include "geometry.h"

namespace FR
{
	FE::Matrix<float> viewport(int x, int y, int w, int h, const int d)
	{
		//std::cout << x << " " << y << " " << w << " " << h << " " << d << std::endl;
		FE::Matrix<float> m = FE::Matrix<float>::identity(4);
		m(0, 3) = x + w / 2.f;
		m(1, 3) = y + h / 2.f;
		m(2, 3) = d / 2.f;

		m(0, 0) = w / 2.f;
		m(1, 1) = h / 2.f;
		m(2, 2) = d / 2.f;
		
		return m;
	}

	/*
		This function should be called before all other ones, beacause it initializes zbuffer
	*/
	void FR_init()
	{
		zbuffer = new int[width * height];
		for (int i = 0; i < width * height; i++)
		{
			zbuffer[i] = std::numeric_limits<int>::min();
		}
	}

	TGAImage generate_image()
	{
		TGAImage i(width, height, TGAImage::RGB);

		return i;
	}

	/*
		This is an ordinary realization of Bresenham's line algorithm ( https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm )
		However, as a last argument function takes reference to the vector of Vec3i variables to store all pixels' coordinates


		@param v0 Vec3i variable which contains origin coordinates
		@param v1 Vec3i variable which contains end coordinates
		@param points   reference to the vector of Vec2i variables

		Note, the shape of line doesn't depend on origin and end coordinates arguments position
		=> line(v0, v1, ...) equals line(v1, v0, ...)
	*/
	void line(FE::Vec3i v0, FE::Vec3i v1, std::vector<FE::Vec3i>& points/*, int* zbuffer*/)
	{
		if (v0.y > v1.y) { std::swap(v0, v1); }

		int length = (v1.y - v0.y) == 0 ? 1 : v1.y - v0.y;
		for (int y = v0.y; y <= v1.y; y++)
		{
			int z = v0.z + (v1.z - v0.z) * (y - v0.y) / (float)(v1.y - v0.y);

			float k = (float)(y - v0.y) / length;
			//	float k2 = (float)(y - uv0.y)/length;
			//	std::cout	<<	k	<<	" and length: "	<<	length		<<	std::endl;
			FE::Vec3i _x = v0 + (v1 - v0) * k;

			//	int idx = _x.x + y*width;
			//	zbuffer[idx] = z;
			//	image.set(_x.x,y,color);

			FE::Vec3i _t(_x.x, y, z);
			points.push_back(_t);
		}
	}
	
	/*
		This function rastezises a triangle with given vertexes and color
		This is the worst implementation ever possible

		@param data     FaceData structure which contains face information
		@param &image   refernce to the TGAImage
		@param *model   Model object
	*/
	// TODO: refactor this monster
	void triangle(FaceData data, TGAImage& image, Model* model)
	{	/* Vec3i v0, Vec3i v1, Vec3i v2 */
		FE::Vec3i v0 = data.screen_coords[0];
		FE::Vec3i v1 = data.screen_coords[1];
		FE::Vec3i v2 = data.screen_coords[2];

		FE::Vec2i uv0 = data.tex_coords[0];
		FE::Vec2i uv1 = data.tex_coords[1];
		FE::Vec2i uv2 = data.tex_coords[2];

		FE::Vec3f vn0 = data.normal_coords[0];
		FE::Vec3f vn1 = data.normal_coords[1];
		FE::Vec3f vn2 = data.normal_coords[2];

		FE::Vec3f n = (data.world_coords[2] - data.world_coords[0]) ^ (data.world_coords[1] - data.world_coords[0]);

		//	vn0.normalize();
		//	vn1.normalize();
		//	vn2.normalize();

		//Don't care about degenerate triangles
		if (v0.y == v1.y && v0.y == v2.y) return;

		/*
			First of all, one should know that we draw our triangle by its halfs, so to detrmine which half will be rasterized first
			we should sort our vertexes by their y-coordinates(in ascending order)
		*/
		if (v0.y > v1.y) { std::swap(v0, v1); std::swap(uv0, uv1); std::swap(vn0, vn1); }
		if (v0.y > v2.y) { std::swap(v0, v2); std::swap(uv0, uv2); std::swap(vn0, vn2); }
		if (v1.y > v2.y) { std::swap(v1, v2); std::swap(uv1, uv2); std::swap(vn1, vn2); }

		int length = (v2.y == v0.y) ? 1 : v2.y - v0.y;

		/*
		std::cout << "v0" << '\n';
		std::cout << "x:" << v0.x << " y:" << v0.y << '\n';

		std::cout << "\nv1" << '\n';
		std::cout << "x:" << v1.x << " y:" << v1.y << '\n';

		std::cout << "\nv2" << '\n';
		std::cout << "x:" << v2.x << " y:" << v2.y << '\n';
		*/

		std::vector<FE::Vec3i> pointsA;
		std::vector<FE::Vec3i> pointsB;

		//Draw triangle's first half, at the time save all points of lines in pointsA and pointsB vectors
		line(v0, v1, pointsA);
		line(v0, v2, pointsB);

		/*
		std::cout << "POINTS A VECTOR" << '\n';
		for(const auto& a : pointsA)
			std::cout << "x:" << a.x << " y:" << a.y << '\n';

		std::cout << "\nPOINTS B VECTOR" << '\n';
		for(const auto& a : pointsB)
			std::cout << "x:" << a.x << " y:" << a.y << '\n';

		std::cout << pointsA.size() << " " << pointsB.size() << '\n';
		*/

		//Here we start to draw our first half
		int segmentLength = v1.y - v0.y;
		for (const auto& betaVec : pointsB)
		{

			for (const auto& alphaVec : pointsA)
			{

				//If our y-coordinates of both vectors equal it means we are on the proper height to start drawing
				if (betaVec.y == alphaVec.y)
				{
					float alpha = (float)(alphaVec.y - v0.y) / length;
					float beta = (float)(alphaVec.y - v0.y) / segmentLength;

					FE::Vec2i alphaVecTex = uv0 + (uv2 - uv0) * alpha;
					FE::Vec2i betaVecTex = uv0 + (uv1 - uv0) * beta;

					FE::Vec3f alphaNorm = vn0 + (vn2 - vn0) * alpha;
					FE::Vec3f betaNorm = vn0 + (vn1 - vn0) * beta;

					//We detrmines which way to draw(left or right)
					if (alphaVec.x < betaVec.x)
					{
						std::swap(alphaVecTex, betaVecTex);
						std::swap(alphaNorm, betaNorm);
						//Draw a line between two x-coordinates
						for (int x = alphaVec.x; x <= betaVec.x; x++)
						{
							float phi = (alphaVec.x == betaVec.x) ? 1.f : float(x - alphaVec.x) / float(betaVec.x - alphaVec.x);
							FE::Vec2i uvP = alphaVecTex + (betaVecTex - alphaVecTex) * phi;

							FE::Vec3f vnP = alphaNorm + (betaNorm - alphaNorm) * phi;

							int z = std::numeric_limits<int>::min();
							long long idx = x + alphaVec.y * width;
							if ((float)(betaVec.x - alphaVec.x) != 0) z = alphaVec.z + (betaVec.z - alphaVec.z) * (x - alphaVec.x) / (float)(betaVec.x - alphaVec.x);
							vnP.normalize();
							//vnP = vnP * -1.0f;				
							float intensity = (phong == true) ? std::max(0.0f, vnP * light_dir) : std::max(0.0f, n * (light_dir));

							TGAColor color = model->getTex(uvP);
							if (zbuffer[idx] < z)
							{
								zbuffer[idx] = z;
								image.set(x, alphaVec.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity));
							}
						}
					}
					else
					{
						//Draw a line between two x-coordinates
						for (int x = betaVec.x; x <= alphaVec.x; x++)
						{
							float phi = (alphaVec.x == betaVec.x) ? 1.f : float(x - betaVec.x) / float(alphaVec.x - betaVec.x);
							FE::Vec2i uvP = alphaVecTex + (betaVecTex - alphaVecTex) * phi;

							FE::Vec3f vnP = alphaNorm + (betaNorm - alphaNorm) * phi;

							int z = std::numeric_limits<int>::min();
							long long idx = x + alphaVec.y * width;
							if ((float)(alphaVec.x - betaVec.x) != 0) z = betaVec.z + (alphaVec.z - betaVec.z) * (x - betaVec.x) / (float)(alphaVec.x - betaVec.x);
							vnP.normalize();
							//vnP = vnP * -1.0f;								
							float intensity = (phong == true) ? std::max(0.0f, vnP * light_dir) : n * (light_dir * (-1));

							TGAColor color = model->getTex(uvP);
							if (zbuffer[idx] < z)
							{
								zbuffer[idx] = z;
								image.set(x, alphaVec.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity));
							}// if (zbuffer[idx] < z)
						}// for (int x = betaVec.x; x <= alphaVec.x; x++)
					}// if (alphaVec.x < betaVec.x)
				}// if (betaVec.y == alphaVec.y)
			}// for (const auto& alphaVec : pointsA)
		}// for (const auto& betaVec : pointsB)

		pointsA.clear();
		pointsB.clear();

		//Draw triangle's second half, at the time same save all points of lines in pointsA and pointsB vectors
		line(v2, v1, pointsA);
		line(v2, v0, pointsB);

		/*
		std::cout << "POINTS A VECTOR" << '\n';
		for(const auto& a : pointsA)
			std::cout << "x:" << a.x << " y:" << a.y << '\n';

		std::cout << "\nPOINTS B VECTOR" << '\n';
		for(const auto& a : pointsB)
			std::cout << "x:" << a.x << " y:" << a.y << '\n';
		*/

		//This drawing loop is actually the same as the first one
		segmentLength = v2.y - v1.y;
		for (const auto& betaVec : pointsB)
		{
			for (const auto& alphaVec : pointsA)
			{
				if (betaVec.y == alphaVec.y)
				{
					float alpha = (float)(alphaVec.y - v0.y) / length;
					float beta = (float)((alphaVec.y - v0.y) - (v1.y - v0.y)) / segmentLength;

					FE::Vec2i alphaVecTex = uv0 + (uv2 - uv0) * alpha;
					FE::Vec2i betaVecTex = uv1 + (uv2 - uv1) * beta;

					FE::Vec3f alphaNorm = vn0 + (vn2 - vn0) * alpha;
					FE::Vec3f betaNorm = vn1 + (vn2 - vn1) * beta;

					if (alphaVec.x < betaVec.x)
					{
						std::swap(alphaVecTex, betaVecTex);
						std::swap(alphaNorm, betaNorm);
						for (int x = alphaVec.x; x <= betaVec.x; x++)
						{
							float phi = (alphaVec.x == betaVec.x) ? 1.f : float(x - alphaVec.x) / float(betaVec.x - alphaVec.x);
							FE::Vec2i uvP = alphaVecTex + (betaVecTex - alphaVecTex) * phi;

							FE::Vec3f vnP = alphaNorm + (betaNorm - alphaNorm) * phi;

							int z = std::numeric_limits<int>::min();
							if ((float)(betaVec.x - alphaVec.x) != 0) z = alphaVec.z + (betaVec.z - alphaVec.z) * (x - alphaVec.x) / (float)(betaVec.x - alphaVec.x);
							long long idx = x + alphaVec.y * width;

							vnP.normalize();
							//vnP = vnP * -1.0f;								
							float intensity = std::max(0.0f, vnP * light_dir);

							TGAColor color = model->getTex(uvP);
							if (zbuffer[idx] < z)
							{
								zbuffer[idx] = z;
								image.set(x, alphaVec.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity));
							}

						}
					}
					else
					{

						for (int x = betaVec.x; x <= alphaVec.x; x++)
						{
							float phi = (alphaVec.x == betaVec.x) ? 1.f : float(x - betaVec.x) / float(alphaVec.x - betaVec.x);
							FE::Vec2i uvP = alphaVecTex + (betaVecTex - alphaVecTex) * phi;

							FE::Vec3f vnP = alphaNorm + (betaNorm - alphaNorm) * phi;

							int z = std::numeric_limits<int>::min();
							if ((float)(alphaVec.x - betaVec.x) != 0) z = betaVec.z + (alphaVec.z - betaVec.z) * (x - betaVec.x) / (float)(alphaVec.x - betaVec.x);
							long long idx = x + alphaVec.y * width;

							vnP.normalize();
							//vnP = vnP * -1.0f;								
							float intensity = std::max(0.0f, vnP * light_dir);

							TGAColor color = model->getTex(uvP);
							if (zbuffer[idx] < z)
							{
								zbuffer[idx] = z;
								image.set(x, alphaVec.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity));
							}// if (zbuffer[idx] < z)
						}// for (int x = betaVec.x; x <= alphaVec.x; x++)
					}// if (alphaVec.x < betaVec.x)
				}// if (betaVec.y == alphaVec.y)
			}// for (const auto& alphaVec : pointsA)
		}// for (const auto& betaVec : pointsB)
	}
	
	void draw_image(Model* model, const char* out_filename)
	{
		FE::Matrix<float> Projection = FE::Matrix<float>::identity(4);
		FE::Matrix<float> Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
		Projection(3, 2) = -1.f / camera.z;

		//std::cout << Viewport << std::endl;
		//std::cout << Projection << std::endl;

		TGAImage image = generate_image();
		//TGAImage image(width, height, TGAImage::RGB);

		//Here we parse through our model
		//At first we loop through all faces in our model
		for (int i = 0; i < model->nvfaces(); i++)
		{
			FaceData data;
			data.vface = model->vface(i);
			data.tface = model->tface(i);
			data.nface = model->nface(i);

			//At second we loop through all vertexes of the face, translating them into screen coordinares
			//according to the TGA configurations ( https://drive.google.com/open?id=1041061LS3waENZWf3ROwKcLRfEUypOc4 )
			for (int j = 0; j < 3; j++)
			{
				FE::Vec3f v = model->vert(data.vface[j]);
				FE::Vec2i vt = model->vtex(data.tface[j]);
				FE::Vec3f vn = model->norm(data.nface[j]);

				FE::Matrix<float> mV = v.toMatrix();
				FE::Vec3f tmp = (Viewport * Projection * mV).toVec3f();
				//std::cout << tmp << std::endl;

				data.screen_coords[j] = FE::Vec3i(tmp.x, tmp.y, tmp.z) /*Vec3i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2., (v.z + 1.) * depth / 2.)*/;
				data.world_coords[j] = v;

				data.tex_coords[j] = vt;

				data.normal_coords[j] = vn;
			}

			// Draw triangle
			triangle(data, image, model);
		}
		image.flip_vertically();
		image.write_tga_file(out_filename);
	}
}
