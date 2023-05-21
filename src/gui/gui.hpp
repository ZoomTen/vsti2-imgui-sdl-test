#ifndef VSTFX_GUI_H
#define VSTFX_GUI_H

#include "SDL_events.h"
#include "SDL_render.h"
#include "gui_image.hpp"
#include "imgui.h"
#include <map>

#include "../vst.h"

class VSTFX;

// -------- GUI Class --------

class VSTFX_GUI {
public:
	// API overrides
	VSTFX_GUI(VSTFX *e);
	~VSTFX_GUI();
	virtual bool open(void *ptr);
	virtual bool getRect(Vst::ERect **rect);
	virtual void idle();

protected:
	// custom functions
	void RenderGUI();

	// corresponds to the attached VST's default values
	float fGain_value{0.5}, fRelease_value{0.0};

private:
	bool m_show_some_panel{true};
	bool successful_init{false};
	SDL_Event event{};
	SDL_Window *window{NULL};
	SDL_Renderer *renderer{NULL};

	/*!
	 * \brief The VSTi effect this editor is attached to, should be initialized
	 * at boot.
	 */
	VSTFX *parent{NULL};

	/*!
	 * \brief Should be initialized at startup.
	 */
	std::map<VSTFX_ImageID, VSTFX_Image *> preloaded_images;

	// gui_image.cpp

	/*!
	 * \brief Updates the SDL texture for an Image struct.
	 */
	SDL_Texture *Image2Texture(VSTFX_Image *img,
							   SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

	/*!
	 * \brief Creates a brand new Image struct from an image ID.
	 */
	VSTFX_Image *LoadImage(VSTFX_ImageID id);

	/*!
	 * \brief Adds an Image struct as a regular element
	 */
	void AddImage(VSTFX_Image *img, ImVec2 size, ImVec2 crop_top_left,
				  ImVec2 crop_bottom_right);

	/*!
	 * \brief Adds an Image struct as part of a drawlist -- can be used for
	 * absolute positioning
	 */
	void AddImage(ImDrawList *dl, VSTFX_Image *img, ImVec2 position,
				  ImVec2 size, ImVec2 crop_top_left, ImVec2 crop_bottom_right);

	/*!
	 * \brief Finds the appropriate preloaded image for a given image ID.
	 */
	VSTFX_Image *FindPreloadedImage(VSTFX_ImageID id);

	// gui_widgets.cpp

	static bool MyKnob(const char *label, float *p_value, float v_min,
					   float v_max);
};

#endif
