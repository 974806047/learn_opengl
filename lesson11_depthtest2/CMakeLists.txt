project(11depthtest)
add_executable(11depthtest main.cpp)
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/shader)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/depth.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/depth.vert DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/template1.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/template1.vert DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/template2.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/template2.vert DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_SOURCE_DIR}/image/marble.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/image )
file(COPY ${CMAKE_SOURCE_DIR}/image/metal.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/image )