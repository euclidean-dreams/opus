cmake_minimum_required(VERSION 3.23)
project(opus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wno-dev)

include(lib/cmake-utils/dependency-integrator.cmake)

integrate_cmake_dependency(
        NAME impresario-utils
        GIT_URL git@github.com:impresario-systems/impresario-utils.git
        GIT_TAG origin/dev
)


# SDL
find_package(SDL2 REQUIRED)
include_directories(opus ${SDL2_INCLUDE_DIRS})

set(
        SOURCE_FILES

        src/semiotics/blocks.h
        src/semiotics/paradigm.h
        src/semiotics/semiotics.h

        src/gizmo/RGBColor.h
        src/Opus.cpp src/Opus.h
        src/framework/Libs.h
        src/framework/Constants.h src/SDLFailure.h
        src/gizmo/Tchotchke.cpp src/gizmo/Tchotchke.h src/percipient/CosmographerPercipient.cpp src/percipient/CosmographerPercipient.h

        src/framework/Framework.h src/Default.h

)
add_executable(opus src/main.cpp ${SOURCE_FILES})

target_include_directories(opus PRIVATE src)
target_include_directories(opus PRIVATE src/semiotics)

target_link_libraries(opus PRIVATE impresario-utils)
target_link_libraries(opus PRIVATE ${SDL2_LIBRARIES})
