cmake_minimum_required(VERSION 3.5)
project(MY_OPENGL)

# 引用目录
include_directories(dependences/GLFW/include)
include_directories(dependences/glad/include)
include_directories(dependences/imgui)
# include_directories(dependences/GLEW/include)
# include_directories(dependences/SOIL/src)
include_directories(dependences/image)
include_directories(dependences/)
include_directories(util)

#库目录
# link_directories(dependences/GLEW/2.1.0lib64)
link_directories(dependences/GLFW/lib_mingw)
# link_directories(dependences/SOIL/lib)
#先编译glad
add_subdirectory(dependences/glad)
add_subdirectory(dependences/imgui)
# add_subdirectory(util)

#引用库
link_libraries(libglfw3.a)
# link_libraries(glew32s.lib)
# link_libraries(libSOIL.a)
link_libraries(glad)
link_libraries(imgui)
# link_libraries(util)

# option(STB_IMAGE_IMPLEMENTATION "stb_image.h" ON)

add_subdirectory(lesson1_window)
add_subdirectory(lesson2_triangle)
add_subdirectory(lesson3_uniform)
add_subdirectory(lesson4_texture)
add_subdirectory(lesson5_glm)
add_subdirectory(lesson6_camera)
# add_subdirectory(03texture)
# add_subdirectory(04translate)
# add_subdirectory(05coordinate)
