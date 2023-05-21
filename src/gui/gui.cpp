#include "gui.hpp"
#include "../core.hpp"
#include "../core_parameters.hpp"
#include "SDL.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

VSTFX_GUI::VSTFX_GUI(VSTFX *e) { parent = e; }

VSTFX_GUI::~VSTFX_GUI() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool VSTFX_GUI::getRect(Vst::ERect **erect) {
	// set the editor's dimensions
	static Vst::ERect r = {
		0, 0, // position
		500,  // width
		500   // height
	};
	*erect = &r;
	return true;
}

bool VSTFX_GUI::open(void *ptr) {
	// initialize the GUI
	bool vinit = SDL_VideoInit(NULL);
	if (vinit) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to init video: %s\n",
					 SDL_GetError());
	}

	// use window provided by host
	window = SDL_CreateWindowFrom(ptr);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					 "Unable to open window: %s\n", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					 "Unable to load renderer: %s\n", SDL_GetError());
	}

	successful_init = !vinit && (window != NULL) && (renderer != NULL);

	if (successful_init) {
		ImGui::CreateContext();

		ImGui_ImplSDL2_InitForSDLRenderer(
            window, renderer
        );
        ImGui_ImplSDLRenderer_Init(renderer);

        // disable layout saving
        ImGui::GetIO().IniFilename = NULL;

        // preload images
        for (int i = 0; i < (int)VSTFX_IMG_LEN; i++) {
            preloaded_images[(VSTFX_ImageID) i] = LoadImage((VSTFX_ImageID) i);
        }
		// TODO: I want to do this but the image hides after hiding the window
		// :(
	}
	return true;
}

void VSTFX_GUI::idle() {
    // update routine
    fGain_value = parent->getParameter(kVolume);
    fRelease_value = parent->getParameter(kRelease);

    if (successful_init) {
        // inputs
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        // setup platform
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        // clear background
        SDL_SetRenderDrawColor(
            renderer, 100, 100, 100, 255
            );
        SDL_RenderClear(renderer);

        // execute imgui
        ImGui::NewFrame();

        RenderGUI();

        ImGui::Render();

        // to SDL backbuffer
        ImGui_ImplSDLRenderer_RenderDrawData(
            ImGui::GetDrawData()
        );

        // perform render
        SDL_RenderPresent(renderer);
    }
}

// -------- Own Functions --------

void VSTFX_GUI::RenderGUI() {
    // make it "full screen"
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Root", NULL,
                    ImGuiWindowFlags_NoTitleBar
                    |ImGuiWindowFlags_MenuBar
                    |ImGuiWindowFlags_NoResize
    );

    if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("File", NULL, false, false)){}
        if (ImGui::MenuItem("Edit", NULL, false, false)){}
        if (ImGui::MenuItem("About", NULL, false, false)){}
        ImGui::EndMenuBar();
    }

    { // Header image
        double img_scaling = (io.DisplaySize.x / 56);

        ImGui::BeginChild(
            "Header",
            ImVec2(0, 15 * img_scaling),
            true);

        ImDrawList* dl = ImGui::GetWindowDrawList();

        AddImage(dl, FindPreloadedImage(VSTFX_IMG_TEST),
                 dl->GetClipRectMin(),
                 ImVec2(io.DisplaySize.x, 15 * img_scaling),
                 ImVec2(0, 0), ImVec2(56, 15)
                 );
        ImGui::EndChild();
    }

    { // Main contents
        ImGui::BeginChild("Contents", ImVec2(0, 0));


        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyScroll|ImGuiTabBarFlags_TabListPopupButton;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Basic controls"))
            {
                if (MyKnob((const char*)"Gain", &fGain_value, (float) 0.0, (float) 1.0)) {
                    if (parent != NULL) {
                        parent->setParameter(kVolume, fGain_value);
                    }
                }
                ImGui::SameLine();
                if (MyKnob((const char*)"Release", &fRelease_value, (float) 0.0, (float) 1.0)) {
                    if (parent != NULL) {
                        parent->setParameter(kRelease, fRelease_value);
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::EndChild();
    }

    // ImGui::ShowDemoWindow();
    ImGui::End();
}
