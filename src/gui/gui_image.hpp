#ifndef VSTFX_GUI_IMAGE_H
#define VSTFX_GUI_IMAGE_H

#include "SDL_render.h"

// -------- Image list --------

enum VSTFX_ImageID {
	VSTFX_IMG_TEST = 0,

	VSTFX_IMG_LEN
};

struct VSTFX_Image {
	VSTFX_ImageID id;
	unsigned char *data;
	int width, height, channels;

	SDL_Texture *texture;

	VSTFX_Image()
		: data(NULL), texture(NULL), width(0), height(0), channels(0) {}
};

#endif
