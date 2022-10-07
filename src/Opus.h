#pragma once

#include <SDL.h>
#include "Default.h"
#include "semiotics.h"
#include "gizmo/Tchotchke.h"

namespace opus {

class Opus : public TickingCirculable {
private:
    std::shared_ptr<Arbiter<const Parcel>> glimpsology;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Constants &constants;
    int windowWidth;
    int windowHeight;

    SDL_Texture *createTexture(const ImpresarioSerialization::Glimpse *glimpse);

public:
    explicit Opus(
            std::shared_ptr<Arbiter<const Parcel>> glimpsology,
            Constants &constants
    );

    ~Opus() override;

    void activate() override;

    uint64_t getTickInterval() override;

    bool finished() override;
};

}


