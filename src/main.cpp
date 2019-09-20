#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

Model *model = NULL;
int *zbuffer = NULL;

const int width  = 800;
const int height = 800;
const int depth  = 255;


/*
	This is an ordinary realization of Bresenham's line algorithm ( https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm )
	However, as a last argument function takes reference to the vector of Vec2i variables to store all pixels' coordinates

	@param v0 Vec2i variable which contains origin coordinates
	@param v1 Vec2i variable which contains end coordinates
	@param &image   refernce to the TGAImage
	@param color    TGAColor variable which contains color of the line
	@param points   reference to the vector of Vec2i variables

	Note, the shape of line doesn't depend on origin and end coordinates arguments' position
	=> line(v0,v1, ...) equals line(v1, v0, ...)
*/
void line(Vec3i v0, Vec3i v1, TGAImage &image, TGAColor color, std::vector<Vec3i>& points, int *zbuffer)
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
			int idx = x + y*width;

			int z = v0.z + (v1.z - v0.z) * ((y - v0.y) / (float)(v1.y - v0.y));

			if(zbuffer[idx]<z)
			{
				zbuffer[idx] = z;
				image.set(x,y,color);
			}


			Vec3i _t(x,y,z);
			points.push_back(_t);
		}
		else
		{
			int idx = x + y*width;
			int z = v0.z + (v1.z - v0.z) * ((y - v0.y) / (float)(v1.y - v0.y));

			if(zbuffer[idx]<z)
			{
				/* You may use these here:
				 * image.set(x,y,color);
				 * to see that zbuffer actually works
				*/
				zbuffer[idx] = z;
				image.set(y,x,color);
			}


			Vec3i _t(y,x,z);
			points.push_back(_t);
		}
		error += deltaerr;
		if(error >= deltax)
		{
			y += (v1.y > v0.y ? 1 : -1);
			error -= deltax * 2;
		}
	}
}

/*
	This function rastezises a triangle with given vertexes and color

	@param v0 Vec2i variable which contains 1st vertex coordinates
	@param v1 Vec2i variable which contains 2nd vertex coordinates
	@param v2 Vec2i variable which contains 3rd vertex coordinates
	@param &image   refernce to the TGAImage
	@param color    TGAColor variable which contains color of the triangle

	Note, the shape of triangle doesn't depend on vertexes coordinates arguments' position
	=> triangle(v0,v1,v3 ...) equals triangle(v3,v0,v1 ...) etc..
*/
void triangle(Vec3i v0, Vec3i v1, Vec3i v2, TGAImage &image, TGAColor color, int *zbuffer)
{
	//Don't care about degenerate triangles
	if (v0.y==v1.y && v0.y==v2.y) return;

	/*
		First of all, one should know that we draw our triangle by its halfs, so to detrmine which half will be rasterized first
		we should sort our vertexes by their y-coordinates(in ascending order)
	*/
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

	std::vector<Vec3i> pointsA;
	std::vector<Vec3i> pointsB;

	//Draw triangle's firs half, at the time save all points of lines in pointsA and pointsB vectors
	line(v0, v1, image, color, pointsA, zbuffer);
	line(v0, v2, image, color, pointsB, zbuffer);

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
	for(const auto& betaVec: pointsB)
	{
		for(const auto& alphaVec: pointsA)
		{
			//If our y-coordinates of both vectors equal it means we are on the proper height to start drawing
			if (betaVec.y == alphaVec.y)
			{
				//We detrmines which way to draw(left or right)
				if(alphaVec.x < betaVec.x)
				{
					//Draw a line between two x-coordinates
					for (int x = alphaVec.x; x <= betaVec.x; x++)
					{
						float t = (x-alphaVec.x)/(float)(betaVec.x-alphaVec.x);
						int z =  alphaVec.z*(1.-t) + betaVec.z*t;

						int idx = x + alphaVec.y*width;

						if(zbuffer[idx]<z)
						{
							zbuffer[idx] = z;
							image.set(x, alphaVec.y, color);
						}

					}
				}
				else
				{
					//Draw a line between two x-coordinates
					for (int x = betaVec.x; x <= alphaVec.x; x++)
					{
						float t = (x-betaVec.x)/(float)(alphaVec.x-betaVec.x);
						int z =  betaVec.z*(1.-t) + alphaVec.z*t;

						int idx = x + alphaVec.y*width;

						if(zbuffer[idx]<z)
						{
							zbuffer[idx] = z;
							image.set(x, alphaVec.y, color);
						}

					}
				}
			}
		}
	}


	pointsA.clear();
	pointsB.clear();

	//Draw triangle's second half, at the time same save all points of lines in pointsA and pointsB vectors
	line(v2, v1, image, color, pointsA, zbuffer);
	line(v2, v0, image, color, pointsB, zbuffer);

	/*
	std::cout << "POINTS A VECTOR" << '\n';
	for(const auto& a : pointsA)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';

	std::cout << "\nPOINTS B VECTOR" << '\n';
	for(const auto& a : pointsB)
		std::cout << "x:" << a.x << " y:" << a.y << '\n';
	*/

	//This drawing loop is actually the same as the first one
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
						float t = (x-alphaVec.x)/(float)(betaVec.x-alphaVec.x);
						int z =  alphaVec.z*(1.-t) + betaVec.z*t;

						int idx = x + alphaVec.y*width;

						if(zbuffer[idx]<z)
						{
							zbuffer[idx] = z;
							image.set(x, alphaVec.y, color);
						}

					}
				}
				else
				{
					for (int x = betaVec.x; x <= alphaVec.x; x++)
					{
						float t = (x-betaVec.x)/(float)(alphaVec.x-betaVec.x);
						int z =  betaVec.z*(1.-t) + alphaVec.z*t;

						int idx = x + alphaVec.y*width;

						if(zbuffer[idx]<z)
						{
							zbuffer[idx] = z;
							image.set(x, alphaVec.y, color);
						}
					}
				}
			}
		}
	}



}

int main(int argc, char** argv)
{
		//Load our model
		if(argc == 2)
			model = new Model(argv[1]);
		else
			model = new Model("obj/default.obj");

		zbuffer = new int[width*height];
		for (int i=0; i<width*height; i++)
		{
			zbuffer[i] = std::numeric_limits<int>::min();
		}



    TGAImage image(width, height, TGAImage::RGB);
		Vec3f light_dir(0,0,-1);

		//Here we parse through our model
		//At first we loop through all faces in our model
		for (int i=0; i<model->nfaces(); i++)
		{
    	std::vector<int> face = model->face(i);
      Vec3i screen_coords[3];
			Vec3f world_coords[3];
			//At second we loop through all vertexes of the face, translating them into screen coordinares
			//according to the TGA configurations ( https://drive.google.com/open?id=1041061LS3waENZWf3ROwKcLRfEUypOc4 )
      for (int j=0; j<3; j++)
			{
        Vec3f v = model->vert(face[j]);
        screen_coords[j] = Vec3i((v.x+1.)*width/2., (v.y+1.)*height/2., (v.z+1.)*depth/2.);
				world_coords[j] = v;
      }
			Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
      n.normalize();
      float intensity = n*light_dir;
			//Draw triangle with random color
      if (intensity>0) triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255), zbuffer);
    }

		// Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
	 	// Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
	 	// Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
		//
		// triangle(t0[0], t0[1] ,t0[2], image, red);
		// triangle(t1[0], t1[1] ,t1[2], image, white);
		// triangle(t2[0], t2[1] ,t2[2], image, green);

		image.flip_vertically();
		image.write_tga_file("output.tga");

  	return 0;
}
