cmake_minimum_required(VERSION 3.10)
project(NiblackBinarization)

set(CMAKE_CXX_STANDARD 17)

# Add LodePNG
add_subdirectory(lodepng)

add_executable(niblack niblack.cpp)
target_link_libraries(niblack PRIVATE lodepng)
