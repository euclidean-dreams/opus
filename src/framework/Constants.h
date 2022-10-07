#pragma once


#include "Libs.h"

namespace opus {

class Constants : public NonCopyable {
public:
    std::string percipiaEndpoint = Config::getInstance().getString("percipiaEndpoint");
    int percipiaWidth = Config::getInstance().getInt("percipiaWidth");
    int percipiaHeight = Config::getInstance().getInt("percipiaHeight");
    int pixelSize = Config::getInstance().getInt("pixelSize");
    bool fullscreen = Config::getInstance().getBool("fullscreen");
};

}


