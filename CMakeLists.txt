cmake_minimum_required(VERSION 2.8)

project(coinDetector)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB COIN_SRC
    "src/*.h"
    "src/*.cpp"
    "src/*.c"
)

add_executable(coinDetector ${COIN_SRC})

target_link_libraries(coinDetector ${OpenCV_LIBS})
