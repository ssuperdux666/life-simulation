cmake_minimum_required(VERSION 3.15)
project(lifeSim)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g -O0)

include_directories(${CMAKE_SOURCE_DIR}/raylib/include)
link_directories(${CMAKE_SOURCE_DIR}/raylib/lib)

add_executable(
    lifeSim
    main.cpp
    src/camera.cpp
    src/creature.cpp
    src/Network.cpp
    src/food.cpp
    src/foodRegion.cpp
    src/game.cpp
    )

target_link_libraries(lifeSim
    raylib
    opengl32
    gdi32
    winmm
)

