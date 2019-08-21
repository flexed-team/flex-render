#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 100;
const int height = 100;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{	
	bool steep = false;
	if ( std::abs(x0 - x1) < std::abs(y0 - y1) ) 
	{
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
	
	if (x0 > x1) 
	{
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
	
	int deltax = std::abs(x1 - x0);
	int deltay = std::abs(y1 - y0);
	
	int error = 0;
	int deltaerr = deltay * 2;
	
	int y = y0;
	
	for(int x = x0; x <= x1; x++)
	{
		if (!steep)
			image.set(x,y,color);
		else 
			image.set(y,x,color);
		error += deltaerr;
		if(error >= deltax)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= deltax * 2;
		}
	}
}

void triangle(Vec2f &v1, Vec2f &v2, Vec2f &v3, TGAImage &image, TGAColor color)
{
	line(v1.x, v1.y, v2.x, v2.y, image, color);
	line(v2.x, v2.y, v3.x, v3.y, image, color);
	line(v3.x, v3.y, v1.x, v1.y, image, color);
}

int main(int argc, char** argv) 
{
		if(argc == 2)
			model = new Model(argv[1]);
		else
			model = new Model("obj/default.obj");
	
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
		Vec2f v0(25,70);
		Vec2f v1(50,85);
		Vec2f v2(85,25);
		triangle(v0, v1 ,v2, image, red);
		
        image.flip_vertically(); 
        image.write_tga_file("output.tga");
        return 0;
}