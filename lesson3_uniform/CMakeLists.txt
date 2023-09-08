project(03uniform)
add_executable(03uniform main.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/obj.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/obj.vert DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader)