#pragma once

#define PROJECT_NAMESPACE opus

/////////////////////////////
///////// make libs available
//////////////////////
#include "ImpresarioUtils.h"

using namespace impresarioUtils;
using namespace ImpresarioSerialization;

////////////////////////
/////////// helpful macros
/////////////////
#define up std::unique_ptr
#define mkup std::make_unique
#define sp std::shared_ptr
#define mksp std::make_shared
#define mv move

#define str std::string
#define vec std::vector
#define lst std::list
#define umap std::unordered_map

#define cast static_cast

#define cfg Config::getInstance()

///////////////////
////// constants
///////////
namespace PROJECT_NAMESPACE {

extern std::string percipia_endpoint;
extern int percipia_width;
extern int percipia_height;
extern int pixel_size;
extern bool fullscreen;

}
