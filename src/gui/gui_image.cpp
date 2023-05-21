#include "gui_image.hpp"
#include "SDL_log.h"
#include "gui.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#include "../../vendor/stb_image.h"
#include "res/include/image.h"

// -------- Image pointers --------

const unsigned char* VSTFX_ImageData[VSTFX_IMG_LEN] = {
    IMGTEST
};

const unsigned int VSTFX_ImageSizes[VSTFX_IMG_LEN] = {
    IMGTEST_SIZE
};

VSTFX_Image* VSTFX_GUI::LoadImage(VSTFX_ImageID id) {
    assert(id >= VSTFX_IMG_TEST);
    assert(id < VSTFX_IMG_LEN);

    VSTFX_Image* img = new VSTFX_Image;

    img->id = id;

    img->data = stbi_load_from_memory(
        VSTFX_ImageData[id],
        VSTFX_ImageSizes[id],
        &img->width,
        &img->height,
        &img->channels,
        STBI_rgb_alpha
        );

    if (img->data == NULL) {
        return NULL;
    }

    return img;
}

SDL_Texture* VSTFX_GUI::Image2Texture(VSTFX_Image* img, SDL_BlendMode blend_mode) {
    assert(img != NULL);

    if (img->texture == NULL) {
        img->texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC,
            img->width, img->height
            );
        if (!img->texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Unable to load texture: %s\n", SDL_GetError()
                         );
            return NULL;
        }
    }

    SDL_SetTextureBlendMode(img->texture, blend_mode);

    if (SDL_UpdateTexture(img->texture, NULL, img->data, img->width*4) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Unable to update texture %d: %s\n", (int)img->id, SDL_GetError()
                     );
    }

    return img->texture;
}

void VSTFX_GUI::AddImage(VSTFX_Image* img,
                         ImVec2 size,
                         ImVec2 crop_top_left,
                         ImVec2 crop_bottom_right
                         ) {
    if (img == NULL) return;

    SDL_Texture* tx = Image2Texture(img);

    ImVec2 uv0 = ImVec2(
        crop_top_left.x/img->width,
        crop_top_left.y/img->height
        );

    ImVec2 uv1 = ImVec2(
        crop_bottom_right.x/img->width,
        crop_bottom_right.y/img->height
        );

    ImGui::Image(tx, size, uv0, uv1);
}

void VSTFX_GUI::AddImage(ImDrawList* dl,
                         VSTFX_Image* img,
                         ImVec2 position,
                         ImVec2 size,
                         ImVec2 crop_top_left,
                         ImVec2 crop_bottom_right
                         ) {
    if (img == NULL) return;

    SDL_Texture* tx = Image2Texture(img);

    ImVec2 uv0 = ImVec2(
        crop_top_left.x/img->width,
        crop_top_left.y/img->height
        );

    ImVec2 uv1 = ImVec2(
        crop_bottom_right.x/img->width,
        crop_bottom_right.y/img->height
        );

    dl->AddImage(tx,
                 position,
                 ImVec2(position.x + size.x, position.y + size.y),
                 uv0,
                 uv1
    );
}

VSTFX_Image* VSTFX_GUI::FindPreloadedImage(VSTFX_ImageID id) {
    auto img_idx = preloaded_images.find(VSTFX_IMG_TEST);
    if (img_idx != preloaded_images.cend()) {
        return img_idx->second;
    }
    return NULL;
}
