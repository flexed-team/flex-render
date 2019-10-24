#ifndef __WINLIB_H__
#define __WINLIB_H__

#include <X11/Xlib.h> 
#include <X11/Xutil.h>

struct FlexColor
{
	XColor c;
	FlexColor(unsigned short R, unsigned short G, unsigned short B){
		c.red= R*256; c.green = G*256; c.blue = B*256;
	} 
};

class FlexWindow
{
private:
	Display *d;
	Window win;
	GC gc;
	XImage * image;

	XGCValues values;

	int width, height;
	int model_width, model_heigth;
	int screen_n;
	int border_width;
	
	unsigned int valuemask;

	unsigned short format;

	Window create_simple_window(int x, int y);
	GC create_gc();
public:
	enum Format {
		GRAYSCALE=1, RGB=3, RGBA=4
	};

	FlexWindow(int w = 400, int h = 400, unsigned short format=4);

	inline Display * get_display_ptr() { return d; }
	inline Window get_x_window() { return win; }
	inline GC get_x_gc() { 	return gc; }

	inline void show_window() 
	{ 
		XMapWindow(d, win);

      		while(1) {
	    		XEvent e;
	    		XNextEvent(d, &e);		
		
	    		if (e.type == MapNotify)
		  		break;
		}  
	}
	inline void close_window() { XCloseDisplay(d); }
	inline void update_window() { XPutImage(d, win, gc, image, 0, 0, 0, 0, model_width, model_heigth); XFlush(d); }

	void set(int x, int y, FlexColor color);
	void init_model(int w=400, int h=400);
};

#endif //__WINLIB_H__
