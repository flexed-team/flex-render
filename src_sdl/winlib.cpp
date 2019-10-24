#include <iostream>
#include <cstdlib>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "winlib.h" 

#define _FLEX_DETAILED_OUTPUT

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
FlexWindow::FlexWindow(const char* title, int w, int h, bool fullscreen) : width(w), height(h)
{	
    	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

    	win = SDL_CreateWindow(
        	title,                  	  	  // window title
        	SDL_WINDOWPOS_UNDEFINED,           	  // initial x position
        	SDL_WINDOWPOS_UNDEFINED,           	  // initial y position
        	width,                             	  // width, in pixels
        	height,                            	  // height, in pixels
        	(fullscreen) ? SDL_WINDOW_FULLSCREEN : 0  // flags
    	);
	
	if (win == NULL) {
        	std::cout<<"Could not create window: " << SDL_GetError() << std::endl;
        	exit(1);
    	}
	std::cout<<"Window width: "<<width<<" "<<" height: "<<height<<std::endl;
	
	rndr = SDL_CreateRenderer
        (
        	win,					   // window ref
        	-1,					   // index of driver
        	SDL_RENDERER_ACCELERATED		   // flags
        );

	#ifdef _FLEX_DETAILED_OUTPUT
    		SDL_RendererInfo info;
    		SDL_GetRendererInfo( rndr, &info );
    		std::cout << "Renderer name: " << info.name << std::endl;
    		std::cout << "Texture formats: " << std::endl;
		for( Uint32 i = 0; i < info.num_texture_formats; i++ )
    		{
        		std::cout << SDL_GetPixelFormatName( info.texture_formats[i] ) << std::endl;
    		}
	#endif

}

FlexWindow::~FlexWindow(){}

void FlexWindow::init_model(int w, int h, int f)
{
	bytespp = f;
	model_width = w; model_height = h;

	image = SDL_CreateTexture
        (
        	rndr,
        	SDL_PIXELFORMAT_ARGB8888,
        	SDL_TEXTUREACCESS_STREAMING,
        	model_width, model_height
        );
	
	buffer.reserve(model_width * model_height * bytespp);
}

void FlexWindow::set(int x, int y, FlexColor color)
{	
	color.val = (((unsigned long)color.r) << 16) + (((unsigned long)color.g) << 8) + (unsigned long)color.b;
        
        const unsigned int offset = ( model_width * color.bytespp * y ) + x * color.bytespp;
        buffer[ offset + 0 ] = color.r;        // b
        buffer[ offset + 1 ] = color.g;        // g
        buffer[ offset + 2 ] = color.b;        // r
        buffer[ offset + 3 ] = color.a;        // a
}

void FlexWindow::update_window()
{
	SDL_UpdateTexture
        (
		image,
            	NULL,
            	&buffer[0],
            	model_width * bytespp
        );

        SDL_RenderCopy(rndr, image, NULL, NULL);
        SDL_RenderPresent(rndr);
}

void FlexWindow::render_fps(Uint32 fps)
{
	TTF_Font* Sans = TTF_OpenFont("fonts/Sans.ttf", 15);

	if(Sans == NULL)
		std::cout<<"Font didn't load: "<<TTF_GetError()<<std::endl;

	SDL_Color White = {255, 255, 255};

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); 

	SDL_Texture* Message = SDL_CreateTextureFromSurface(rndr, surfaceMessage); 

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 1;  //controls the rect's x coordinate 
	Message_rect.y = 1; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 100; // controls the height of the rect

	SDL_RenderCopy(rndr, Message, NULL, &Message_rect);
	
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}
// PRIVATE / PROTECTED

