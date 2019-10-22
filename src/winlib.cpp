#include <iostream>
#include <cstdlib>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "winlib.h" 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
FlexWindow::FlexWindow(int w, int h, unsigned short format) : d(NULL), gc(NULL), width(w), height(h), screen_n(0), border_width(0), valuemask(0)
{
	XInitThreads();
	const char *display_name = std::getenv("DISPLAY");
	d = XOpenDisplay(display_name);
	if(d == NULL)
	{
		std::cerr<<"Cannot connect to X server"<<std::endl;
		std::exit(EXIT_FAILURE);
	}

	screen_n = DefaultScreen(d);
	std::cout<<"window width: "<<width<<" "<<" height: "<<height<<std::endl;

	win = create_simple_window(1, 1);

	XSelectInput(d, win, StructureNotifyMask);

	gc = create_gc();

	XSetForeground(d, gc, BlackPixel(d, screen_n));
	XSetBackground(d, gc, BlackPixel(d, screen_n));

 	XSetFillStyle(d, gc, FillSolid);
}

void FlexWindow::init_model(int w, int h)
{
	model_width = w; model_heigth = h;
	
	image = XGetImage(d, RootWindow(d, screen_n), 0, 0, model_width, model_heigth, AllPlanes, ZPixmap);

	int y = 0;
    	while (y < model_heigth) {  
        	int x = 0;
        	while (x < model_width) {
            		XPutPixel(image, x, y, BlackPixel(d, screen_n));
            		x++;
        	}    
        	y++;
    	}

    	//image = XCreateImage(d, 
        //	DefaultVisual(d, screen_n),
        //	DefaultDepth(d, screen_n), 
        //	ZPixmap, 0, data, w, h, 32, 0);
}

void FlexWindow::set(int x, int y, FlexColor color)
{	
	//Colormap colormap = DefaultColormap(d, 0);
	//XAllocColor(d, colormap, &color.c);
	color.c.pixel = (((unsigned long)color.c.red/256) << 16) + (((unsigned long)color.c.green/256) << 8) + (unsigned long)color.c.blue/256;

        XPutPixel(image, x, y, color.c.pixel);
	
	//XSetForeground(d, gc, color.c.pixel);
	
	//XDrawPoint(d, win, gc, x, y);
}
// PRIVATE / PROTECTED

Window FlexWindow::create_simple_window(int x, int y)
{
  	Window w;
	XSetWindowAttributes setwinattr;
	unsigned long valuemask = CWBackPixel | CWBorderPixel;

	setwinattr.background_pixel = BlackPixel(d, screen_n);
	setwinattr.border_pixel = BlackPixel(d, screen_n);	

 	w = XCreateWindow(d, DefaultRootWindow(d),
        		  x, y, width, height, border_width, DefaultDepth(d, screen_n),
			  InputOutput, DefaultVisual(d, screen_n),
			  valuemask, &setwinattr);

  	XFlush(d);

	return w;
}

GC FlexWindow::create_gc()
{
	GC _gc;	

	_gc = XCreateGC(d, win, valuemask, &values);
	if(gc < 0)
		std::cerr<<"X create GC: "<<std::endl;

	return _gc;
}

