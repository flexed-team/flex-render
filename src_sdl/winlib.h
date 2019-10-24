#ifndef __WINLIB_H__
#define __WINLIB_H__

#include <vector>

#include <X11/Xlib.h> 
#include <X11/Xutil.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

struct FlexColor
{
        union
	{
		struct{
			unsigned char r, g, b, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	FlexColor(unsigned short R, unsigned short G, unsigned short B, unsigned short A=255): r(R), g(G), b(B), a(A), bytespp(4){
	}
};

class FlexWindow
{
private:
	SDL_Window *win;
	SDL_Renderer *rndr;
	SDL_Texture *image;

	std::vector<unsigned char> buffer;
	
	int bytespp = 4; 
	int width, height;
	int model_width, model_height;
public:
	enum Format {
		GRAYSCALE=1, RGB=3, RGBA=4
	};

	FlexWindow(const char* title, int w = 400, int h = 400, bool fullscreen=false);
	~FlexWindow();

	inline int get_screen_width()  { return width; };
	inline int get_screen_height() { return height; };
	inline int get_img_width()     { return model_width; };
	inline int get_img_height()    { return model_height; };
	
	inline void close_window() { SDL_DestroyWindow(win); SDL_DestroyRenderer(rndr); }
	inline void clear_window() { SDL_SetRenderDrawColor(rndr, 0, 0, 0, 255); SDL_RenderClear(rndr);}

	void update_window();

	void render_fps(Uint32 fps);

	void set(int x, int y, FlexColor color);
	void init_model(int w=400, int h=400, int f=4);
};

#endif //__WINLIB_H__
