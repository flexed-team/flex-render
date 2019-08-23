#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

Model *model = NULL;
const int width  = 400;
const int height = 400;

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color, std::vector<Vec2i>& points)
{
	bool steep = false;
	if ( std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y) )
	{
        std::swap(v0.x, v0.y);
        std::swap(v1.x, v1.y);
        steep = true;
    }

	if (v0.x > v1.x)
	{
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }

	int deltax = std::abs(v1.x - v0.x);
	int deltay = std::abs(v1.y - v0.y);

	int error = 0;
	int deltaerr = deltay * 2;

	int y = v0.y;

	for(int x = v0.x; x <= v1.x; x++)
	{
		if (!steep)
		{
			image.set(x,y,color);

			Vec2i t(x,y);
			points.push_back(t);
		}
		else
		{
			image.set(y,x,color);

			Vec2i t(y,x);
			points.push_back(t);
		}
		error += deltaerr;
		if(error >= deltax)
		{
			y += (v1.y > v0.y ? 1 : -1);
			error -= deltax * 2;
		}
	}
}

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, TGAColor color)
{
	if (v0.y > v1.y) std::swap(v0, v1);
  if (v0.y > v2.y) std::swap(v0, v2);
  if (v1.y > v2.y) std::swap(v1, v2);

	/*
	std::cout << "v0" << '\n';
	std::cout << "x:" << v0.x << " y:" << v0.y << '\n';

	std::cout << "\nv1" << '\n';
	std::cout << "x:" << v1.x << " y:" << v1.y << '\n';

	std::cout << "\nv2" << '\n';
	std::cout << "x:" << v2.x << " y:" << v2.y << '\n';
	*/

	std::vector<Vec2i> pointsA;
	std::vector<Vec2i> pointsB;

	//line(v1, v2, image, color);
	line(v0, v1, image, color, pointsA);
	line(v0, v2, image, color, pointsB);

	/*
	std::cout << "POINTS A VECTOR" << '\n';
	for(const auto& a : pointsA)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';

	std::cout << "\nPOINTS B VECTOR" << '\n';
	for(const auto& a : pointsB)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';

	std::cout << pointsA.size() << " " << pointsB.size() << '\n';
	*/


	for(const auto& betaVec: pointsB)
	{
		for(const auto& alphaVec: pointsA)
		{
			if (betaVec.y == alphaVec.y)
			{
				if(alphaVec.x < betaVec.x)
				{
					for (int x = alphaVec.x; x <= betaVec.x; x++)
					{
						image.set(x, alphaVec.y, color);
					}
				}
				else
				{
					for (int x = betaVec.x; x <= alphaVec.x; x++)
					{
						image.set(x, alphaVec.y, color);
					}
				}
			}
		}
	}


	pointsA.clear();
	pointsB.clear();

	line(v2, v1, image, color, pointsA);
	line(v2, v0, image, color, pointsB);

	/*
	std::cout << "POINTS A VECTOR" << '\n';
	for(const auto& a : pointsA)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';

	std::cout << "\nPOINTS B VECTOR" << '\n';
	for(const auto& a : pointsB)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';
	*/

	for(const auto& betaVec: pointsB)
	{
		for(const auto& alphaVec: pointsA)
		{
			if (betaVec.y == alphaVec.y)
			{
				if(alphaVec.x < betaVec.x)
				{
					for (int x = alphaVec.x; x <= betaVec.x; x++)
					{
						image.set(x, alphaVec.y, color);
					}
				}
				else
				{
					for (int x = betaVec.x; x <= alphaVec.x; x++)
					{
						image.set(x, alphaVec.y, color);
					}
				}
			}
		}
	}



}

int main(int argc, char** argv)
{
		// if(argc == 2)
		// 	model = new Model(argv[1]);
		// else
		// 	model = new Model("obj/default.obj");

    TGAImage image(width, height, TGAImage::RGB);

		/*for (int i = 0; i < model->nfaces(); i++)
		{
			std::vector<int> face = model->face(i);

			for (int j = 0; j < 3; j++)
			{
				Vec3f v0 = model->vert( face[j] );
				Vec3f v1 = model->vert( face[ (j+1) % 3] );

				int x0 = (v0.x + 1.) * width / 2.;
				int y0 = (v0.y + 1.) * height / 2.;
				int x1 = (v1.x + 1.) * width / 2.;
				int y1 = (v1.y + 1.) * height / 2.;

				line(x0, y0, x1, y1, image, white);
			}
		}*/

		Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
	 	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
	 	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

		triangle(t0[0], t0[1] ,t0[2], image, red);
		triangle(t1[0], t1[1] ,t1[2], image, white);
		triangle(t2[0], t2[1] ,t2[2], image, green);

		image.flip_vertically();
		image.write_tga_file("output.tga");

  	return 0;
}
