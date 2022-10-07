#include "Opus.h"

namespace opus {

Opus::Opus(
        std::shared_ptr<Arbiter<const Parcel>> glimpsology,
        Constants &constants
) :
        glimpsology{move(glimpsology)},
        constants{constants},
        windowWidth{constants.percipiaWidth * constants.pixelSize},
        windowHeight{constants.percipiaHeight * constants.pixelSize} {
    auto initializationResult = SDL_Init(SDL_INIT_VIDEO);
    if (initializationResult != 0) {
        throw SDLFailure{};
    }
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_PING, "0");
    if (constants.fullscreen) {
        window = SDL_CreateWindow("euclid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                  windowHeight,
                                  SDL_WINDOW_BORDERLESS);
    } else {
        window = SDL_CreateWindow("euclid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                  windowHeight, 0);
    }
    Tchotchke::assertNotNull(window);

    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    Tchotchke::assertNotNull(renderer);
}

Opus::~Opus() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Opus::activate() {
    if (glimpsology->newDataAvailable()) {
        auto parcel = glimpsology->take();
        auto glimpse = Unwrap::Glimpse(*parcel);
        SDL_RenderClear(renderer);
        auto texture = createTexture(glimpse);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }
}

SDL_Texture *Opus::createTexture(const ImpresarioSerialization::Glimpse *glimpse) {
    Uint32 redMask, greenMask, blueMask, alphaMask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    redMask = 0xff000000;
    greenMask = 0x00ff0000;
    blueMask = 0x0000ff00;
    alphaMask = 0x000000ff;
#else
    redMask = 0x000000ff;
    greenMask = 0x0000ff00;
    blueMask = 0x00ff0000;
    alphaMask = 0xff000000;
#endif
    auto surface = SDL_CreateRGBSurface(0, windowWidth, windowHeight, 32, redMask, greenMask, blueMask,
                                        alphaMask);
    Tchotchke::assertNotNull(surface);

    auto colors = glimpse->colors();
    auto pixels = (Uint32 *) surface->pixels;
    for (int index = 0; index < colors->size(); index++) {
        auto glimpseColor = (*colors)[index];
        auto surfaceColor = SDL_MapRGBA(surface->format, glimpseColor->red(), glimpseColor->green(),
                                        glimpseColor->blue(), 255);
        auto glimpseX = index % constants.percipiaWidth;
        auto glimpseY = (int) std::floor(index / constants.percipiaWidth);
        auto surfaceLowX = glimpseX * constants.pixelSize;
        auto surfaceLowY = glimpseY * constants.pixelSize;
        auto surfaceHighX = surfaceLowX + constants.pixelSize - 1;
        auto surfaceHighY = surfaceLowY + constants.pixelSize - 1;
        for (int y = surfaceLowY; y <= surfaceHighY; y++) {
            for (int x = surfaceLowX; x <= surfaceHighX; x++) {
                auto pixelIndex = windowWidth * y + x;
                pixels[pixelIndex] = surfaceColor;
            }
        }
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    Tchotchke::assertNotNull(texture);
    SDL_FreeSurface(surface);

    return texture;
}

uint64_t Opus::getTickInterval() {
    return 500;
}

bool Opus::finished() {
    return false;
}

}
