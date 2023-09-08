file(GLOB SRC
    ./*.h
    ./*.cpp
)
add_library(imgui ${SRC})